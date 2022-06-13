///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup wifi wifi
/// @{

/// @file co_bmec_wifi.cpp
/// @brief Wifi module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include <cstdint>
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include <WiFi.h>
#include <BLEDevice.h>
#include <Arduino_JSON.h>
#include <ESP32Ping.h>

#include "co_bmec_wifi.h"

//*********************************************************************
// defines.
//*********************************************************************
#define WIFI_TASK_STACK_SIZE         4000  /// Used 2360 -> 4000. The number of wifi networks may affect this so a large margin of error is required.

#define WIFI_IDLE_TIME_MS            100

#define PREF_NS_WIFI_CONFIG     "WIFI_CONFIG"
#define CONNECT_MAX_BACKOFF_MS  32000

//*********************************************************************
// #constructors.
//*********************************************************************

//*********************************************************************
// implementations.
//*********************************************************************

// Constructors.

/// Initialises the wifi module and BLE callbacks.
/// @param core Core on which to run the FreeRTOS task.
/// @param priority Priority at which to run the FreeRTOS task.
/// @param flashMutex Global flash mutex.
/// @param bleServiceWifi wifi ble service.
/// @param ref pointer to pass back in the callback.
/// @param apStateCallback callback. Caller can pass reference through public void* ref_.
CoBmecWifi::CoBmecWifi(
    void *ref,
    int core,
    int priority,
    SemaphoreHandle_t flashMutex,
    BleServiceWifi *bleServiceWifi,
    void (*apStateCallback)(CoBmecWifi *, ApState))
    : ref_(ref),
      core_(core),
      priority_(priority),
      flashMutex_(flashMutex),
      bleServiceWifi_(bleServiceWifi),
      apStateCallback_(apStateCallback) {}

// Public methods.

/// Initialise the module and ble interface.
void CoBmecWifi::init() {
  log_i("CoBmecWifi module init");

  // Load the Wi-Fi configuration from NVS.
  loadConfig();

  // Set ble callbacks.
  bleServiceWifi_->charApCommand_->setCallbacks(this);
  bleServiceWifi_->charScanCommand_->setCallbacks(this);

  // Set the Wi-Fi listener (see onWrite).
  (void) WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
    if (!xQueueSend(wifiEventQueue_,
                    &event,
                    WIFI_EVENT_QUEUE_IDLE_TIME_MS / portTICK_PERIOD_MS)) {
      log_w("failed to add item to wifiEventQueue_ queue."); // NOLINT(bugprone-lambda-function-name)
    }
  });

  // Enable auto reconnect.
  WiFi.setAutoReconnect(false);

  // Create the background task.
  (void) xTaskCreatePinnedToCore(task, // Function to implement the task
                                 "CoBmecWifi", // Name of the task
                                 WIFI_TASK_STACK_SIZE,  // Stack size in words
                                 this,  // Task input parameter
                                 priority_,  // Priority of the task
                                 nullptr,  // Task handle.
                                 core_); // Core where the task should run

  log_i("CoBmecWifi module inited");
}

/// Initialises the wifi module and BLE callbacks.
/// @returns the rssi of the connected wifi AP in dBm.
long CoBmecWifi::getRssiDbm() {
  return WiFi.RSSI();
}

// Private methods.

/// Sets the scanning state, notifies on the BLE and calls the callback.
void CoBmecWifi::setApState(CoBmecWifi::ApState apState) {

  // Log state.
  switch (apState) {
    case ApState::UNDEFINED:log_i("ApState: UNDEFINED");
      break;
    case ApState::DISCONNECTING:log_i("ApState: DISCONNECTING");
      break;
    case ApState::DISCONNECTED:log_i("ApState: DISCONNECTED");
      break;
    case ApState::CONNECTING:log_i("ApState: CONNECTING");
      break;
    case ApState::CONNECTED:log_i("ApState: CONNECTED");
      break;
    case ApState::PINGING:log_i("ApState: PINGING");
      break;
    case ApState::PINGED:log_i("ApState: PINGED");
      break;
    case ApState::ERROR:log_i("ApState: ERROR");
      break;
  }

  // Set the state.
  apState_ = apState;
  // Notify on the ble characteristic.
  bleServiceWifi_->charApState_
                 ->setValue(reinterpret_cast<uint8_t *>(&apState_),
                            1);
  bleServiceWifi_->charApState_->notify();

  // Callback.
  apStateCallback_(this,
                   apState);
}

/// Sets AP state to error and notifies on the BLE.
void CoBmecWifi::setApError(const String &errorMessage) {
  // Log error.
  log_w("AP Error: %s",
        errorMessage.c_str());

  // Set the state.
  setApState(ApState::ERROR);
  // Notify on the ble characteristic.
  bleServiceWifi_->charApError_->setValue(errorMessage.c_str());
}

/// Sets the scanning state and notifies on the BLE.
void CoBmecWifi::setScanState(CoBmecWifi::ScanState scanState) {
  // Log state.
  switch (scanState) {
    case ScanState::UNDEFINED:log_i("ScanState: UNDEFINED");
      break;
    case ScanState::SCANNING:log_i("ScanState: SCANNING");
      break;
    case ScanState::SCANNED:log_i("ScanState: SCANNED");
      break;
    case ScanState::ERROR:log_i("ScanState: ERROR");
      break;
  }
  // Set the state.
  scanState_ = scanState;
  // Notify on the ble characteristic.
  bleServiceWifi_->charScanState_
                 ->setValue(reinterpret_cast<uint8_t *>(&scanState_),
                            1);
  bleServiceWifi_->charScanState_->notify();
}

/// Sets the scanning error.
void CoBmecWifi::setScanError(const String &errorMessage) {
  // Log error.
  log_w("Scan Error: %s",
        errorMessage.c_str());

  // Set the state.
  setScanState(ScanState::ERROR);
  // Notify on the ble characteristic.
  bleServiceWifi_->charScanError_->setValue(errorMessage.c_str());
}

/// Callback for events from WiFi driver.
void CoBmecWifi::onWifiEvent(WiFiEvent_t event) {
  //FEATURE param WiFiEventInfo_t info can be used to improve specificity of state.

  switch (static_cast<system_event_id_t>(event)) {
    case SYSTEM_EVENT_WIFI_READY: {
      log_i("WiFi ready.");
    }
      break;
    case SYSTEM_EVENT_SCAN_DONE: {
      log_i("WiFi scan done.");
      onScanDone();
    }
      break;
    case SYSTEM_EVENT_STA_START: {
      log_i("WiFi started.");
      setApState(ApState::DISCONNECTED);
    }
      break;
    case SYSTEM_EVENT_STA_STOP: {
      log_i("WiFi stopped.");
    }
      break;
    case SYSTEM_EVENT_STA_CONNECTED: {
      log_i("Connected to WiFi access point.");
      // Do nothing - wait for IP.
    }
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED: {
      log_i("Disconnected from WiFi access point.");
      setApState(ApState::DISCONNECTED);
    }
      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE: {
      log_i("Authentication mode of access point has changed.");
      setApState(ApState::DISCONNECTED);
    }
      break;
    case SYSTEM_EVENT_STA_GOT_IP: {
      log_i("Obtained IP address: %s",
            WiFi.localIP().toString().c_str());
      saveConfig();
      setApState(ApState::CONNECTED);
    }
      break;
    case SYSTEM_EVENT_STA_LOST_IP: {
      log_i("Lost IP address and IP address is reset to 0.");
      setApState(ApState::DISCONNECTED);
    }
      break;
    default: {
      log_e("Unexpected wifi event.");
    }
      break;
  }
}

/// Callback for write events from the ble driver.
void CoBmecWifi::onWrite(BLECharacteristic *characteristic) {

  // AP commands.
  if (characteristic == bleServiceWifi_->charApCommand_) {
    switch ((ApCommand) bleServiceWifi_->charApCommand_->getValue()
                                       .c_str()[0]) {
      case ApCommand::DISCONNECT: {
        log_i("ApCommand: DISCONNECT");
        wifiDisconnect();
      }
        break;
      case ApCommand::CONNECT: {
        log_i("ApCommand: CONNECT");
        setConfig();
        wifiConnect();
      }
        break;
      case ApCommand::PING: {
        log_i("ApCommand: PING");
        internetPing();
      }
        break;
      default:break;
    }
  }

  // Scanning command.
  if (characteristic == bleServiceWifi_->charScanCommand_) {
    switch ((ScanCommand) bleServiceWifi_->charScanCommand_->getValue()
                                         .c_str()[0]) {
      case ScanCommand::SCAN: {
        log_i("ScanCommand: SCAN");
        if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
          setScanState(ScanState::SCANNING);

          // Wait for any connection attempts to finish before starting a scan.
          while (apState_ == ApState::CONNECTING) {
            vTaskDelay(WIFI_IDLE_TIME_MS / portTICK_PERIOD_MS);
          }

          // Start scan.
          WiFi.scanNetworks(true);

        }
        else {
          log_w("Scan already running.");
        }
      }
        break;
      default:break;
    }
  }
}

/// Loads the mqtt from NVS.
void CoBmecWifi::loadConfig() {

  // Take the mutex.
  xSemaphoreTake(flashMutex_,
                 portMAX_DELAY);

  // Set namespace.
  if (!preferences_.begin(PREF_NS_WIFI_CONFIG)) {
    log_e("Could not init wifi NVS.");
  }

  // Get preferences.
  config_->ssid_ = preferences_.getString("ssid",
                                          DEFAULT_WIFI_SSID);
  config_->security_ = static_cast<wifi_auth_mode_t>(preferences_
      .getUInt("security"), WIFI_AUTH_WPA_WPA2_PSK);
  config_->identity_ = preferences_.getString("identity");
  config_->username_ = preferences_.getString("username");
  config_->password_ = preferences_.getString("password",
                                              DEFAULT_WIFI_PASSWORD);

  preferences_.end();

  // Release the mutex.
  while (!xSemaphoreGive(flashMutex_)) {
    log_e("Failed to give flashMutex_.");
  }

  // Set the values on the BLE.
  bleServiceWifi_->charSsid_->setValue(config_->ssid_.c_str());
  bleServiceWifi_->charSecurity_
                 ->setValue(reinterpret_cast<uint8_t *>(&config_->security_),
                            1);
  bleServiceWifi_->charIdentity_->setValue(config_->identity_.c_str());
  bleServiceWifi_->charUsername_->setValue(config_->username_.c_str());
  bleServiceWifi_->charPassword_->setValue(config_->password_.c_str());

  // Log.
  log_i("status_->ssid_: %s",
        config_->ssid_.c_str());
  log_i("status_->security: %u",
        config_->security_);
  log_i("status_->identity_: %s",
        config_->identity_.c_str());
  log_i("status_->username_: %s",
        config_->username_.c_str());
  log_i("status_->password: %s",
        config_->password_.c_str());
}

/// Sets the wifi mqtt from the BLE.
void CoBmecWifi::setConfig() {

  // Set the values if they have changed.
  String newSsid = bleServiceWifi_->charSsid_->getValue().c_str();
  if (config_->ssid_ != newSsid) {
    config_->ssid_ = newSsid;
    config_->ssidUpdated_ = true;
    log_i("updated ssid_");
  }
  auto newSecurity =
      (wifi_auth_mode_t) bleServiceWifi_->charSecurity_->getData()[0];
  if (config_->security_ != newSecurity) {
    config_->security_ = newSecurity;
    config_->securityUpdated_ = true;
    log_i("updated security");
  }
  String newIdentity = bleServiceWifi_->charIdentity_->getValue().c_str();
  if (config_->identity_ != newIdentity) {
    config_->identity_ = newIdentity;
    config_->identityUpdated_ = true;
    log_i("updated identity_");
  }
  String newUsername = bleServiceWifi_->charUsername_->getValue().c_str();
  if (config_->username_ != newUsername) {
    config_->username_ = newUsername;
    config_->usernameUpdated_ = true;
    log_i("updated username_");
  }
  String newPassword = bleServiceWifi_->charPassword_->getValue().c_str();
  if (config_->password_ != newPassword) {
    config_->password_ = newPassword;
    config_->passwordUpdated_ = true;
    log_i("updated password");
  }

  // Log.
  log_v("status_->ssid_: %s",
        config_->ssid_.c_str());
  log_v("status_->security: %u",
        config_->security_);
  log_v("status_->identity_: %s",
        config_->identity_.c_str());
  log_v("status_->username_: %s",
        config_->username_.c_str());
  log_v("status_->password: %s",
        config_->password_.c_str());
}

/// Saves the wifi mqtt.
void CoBmecWifi::saveConfig() {

  // Take the mutex.
  xSemaphoreTake(flashMutex_,
                 portMAX_DELAY);

  // Set namespace.
  if (!preferences_.begin(PREF_NS_WIFI_CONFIG)) {
    log_e("Could not init wifi NVS.");
  }

  // Set WiFi in preferences.
  if (config_->ssidUpdated_) {
    if (!preferences_.putString("ssid",
                                config_->ssid_)) {
      log_e("Failed to save status_->ssid_ to NVS");
    }
  }
  if (config_->securityUpdated_) {
    if (!preferences_.putUShort("security",
                                static_cast<uint>(config_->security_))) {
      log_e("Failed to save status_->security to NVS");
    }
  }
  if (config_->identityUpdated_) {
    if (!preferences_.putString("identity",
                                config_->identity_)) {
      log_w("Failed to save status_->identity_ to NVS");
    }
  }
  if (config_->usernameUpdated_) {
    if (!preferences_.putString("username",
                                config_->username_)) {
      log_w("Failed to save status_->username_ to NVS");
    }
  }
  if (config_->passwordUpdated_) {
    if (!preferences_.putString("password",
                                config_->password_)) {
      log_e("Failed to save status_->password to NVS");
    }
  }

  // Commit.
  preferences_.end();

  // Release the mutex.
  while (!xSemaphoreGive(flashMutex_)) {
    log_e("Failed to give flashMutex_.");
  }
}

/// Attempts to save clear wifi mqtt.
void CoBmecWifi::deleteConfig() {

  // Take the mutex.
  xSemaphoreTake(flashMutex_,
                 portMAX_DELAY);

  // Set namespace.
  if (!preferences_.begin(PREF_NS_WIFI_CONFIG)) {
    log_e("Could not init wifi NVS.");
  }

  if (!preferences_.clear()) {
    log_e("Could clear wifi NVS.");
  }

  // Commit.
  preferences_.end();

  // Release the mutex.
  while (!xSemaphoreGive(flashMutex_)) {
    log_e("Failed to give flashMutex_.");
  }

  // Set the values.
  config_->ssid_ = "";
  config_->security_ = WIFI_AUTH_OPEN;
  config_->identity_ = "";
  config_->username_ = "";
  config_->password_ = "";

  // Set the values on the BLE.
  bleServiceWifi_->charSsid_->setValue(config_->ssid_.c_str());
  bleServiceWifi_->charSecurity_
                 ->setValue(reinterpret_cast<uint8_t *>(&config_->security_),
                            1);
  bleServiceWifi_->charIdentity_->setValue(config_->identity_.c_str());
  bleServiceWifi_->charUsername_->setValue(config_->username_.c_str());
  bleServiceWifi_->charPassword_->setValue(config_->password_.c_str());

}

/// Attempts to make disconnect wifi connection.
void CoBmecWifi::wifiDisconnect() {

  // Set the state.
  setApState(ApState::DISCONNECTING);

  // Clear from NVS.
  deleteConfig();

  // Disconnect.
  WiFi.disconnect();
}

/// Attempts to make wifi connection.
void CoBmecWifi::wifiConnect() {

  if (config_->ssid_.isEmpty()) {
    log_w("WARNING: Attempted to connect to AP with no ssid_");
    setApState(ApState::DISCONNECTED);
    return;
  }

  setApState(ApState::CONNECTING);

  // Set auto connect. This may be redundant.
//  WiFi.setAutoReconnect(false);

  // Attempt to connect.
  switch (config_->security_) {
    case WIFI_AUTH_OPEN: {
      WiFi.begin(config_->ssid_.c_str());
    }
      break;
    case WIFI_AUTH_WEP:
    case WIFI_AUTH_WPA_PSK:
    case WIFI_AUTH_WPA2_PSK:
    case WIFI_AUTH_WPA_WPA2_PSK: {
      WiFi.begin(config_->ssid_.c_str(),
                 config_->password_.c_str());
    }
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE: {
      // FEATURE this has never been tested.
      esp_wifi_sta_wpa2_ent_set_identity((const unsigned char *) config_
                                             ->identity_.c_str(),
                                         (int) config_->identity_.length());
      esp_wifi_sta_wpa2_ent_set_username((const unsigned char *) config_
                                             ->username_.c_str(),
                                         (int) config_->username_.length());
      esp_wifi_sta_wpa2_ent_set_password((const unsigned char *) config_
                                             ->password_.c_str(),
                                         (int) config_->password_.length());
      esp_wifi_sta_wpa2_ent_enable(); //set mqtt settings to enable function
      WiFi.begin(config_->ssid_.c_str());
    }
      break;
    case WIFI_AUTH_MAX:
    default: {
      log_w("WARNING: Unknown wifi security type.");
    }
  }
}

/// Attempts to ping google.
/// Prevents continuous calls with exponential backoff.
void CoBmecWifi::internetPing() {
  // Init the backoff index.
  static int backoffIndex = 0;
  // Init the last ping time.
  static unsigned long lastPingTMs = 0;
  // Init backoff delay.
  static uint32_t backoffMs = 0;

  // Return if delay has not expired.
  if ((millis() - lastPingTMs) < backoffMs) {
    return;
  }

  // Set the state.
  setApState(ApState::PINGING);

  // Set the last ping.
  lastPingTMs = millis();

  // Attempt ping.
  bool pinged = Ping.ping("8.8.8.8",
                          1);

  // If ping fails set the state back to connected and delay before trying again.
  if (pinged) {
    // Set the state.
    setApState(ApState::PINGED);

    // Reset the backoff index.
    backoffIndex = 0;
    // Reset the last ping time.
    lastPingTMs = 0;
    // Reset backoff delay.
    backoffMs = 0;

  }
  else {
    // Set the state.
    setApState(ApState::CONNECTED);

    // Compute the backoff delay.
    backoffMs = (uint32_t) min(int(pow(2,
                                       backoffIndex++)) * 1000
                                   + int(random(1000)),
                               CONNECT_MAX_BACKOFF_MS);

    // Log the delay.
    log_w("Failed to ping google. Trying again after %lu ms",
          backoffMs);
  }
}

/// Handles completion of wifi scan notifying scan state
/// and writing AP list to BLE.
void CoBmecWifi::onScanDone() {
  // Get the scan result.
  int resultCount = WiFi.scanComplete();
  // Handle the scan result.
  if (resultCount == WIFI_SCAN_FAILED) {
    setScanError("WiFi scan failed.");
  }
  else if (resultCount > 0) {
    log_i("WiFi scan has results.");

    /// Uncomment to test handling of a large number of results and large ble packet size.
    //resultCount = 60; // Uncomment for testing only

    // Get the max packet length.
    int maxPacketLen =
        ESP_GATT_MAX_ATTR_LEN;  //FEATURE this may need to be tested on other devices.

    uint8_t resultIndex = 0;  // Index of result.
    int charApListPartIndex = 1;  // Index of ap list part.

    // Write as many results to each characteristic as will fit inside the maxPacketLen.
    while (resultIndex < resultCount) {
      JSONVar scanResultsJson;  // Holds the array of results.
      String scanResultsString;  // Holds string of JSON.
      JSONVar scanResultJson;  // Holds each result.
      String scanResultString;  // Holds string of JSON.

      // Add the previous loop's result to the array if it fits and exists.
      do {
        // Add to array if not empty.
        if (scanResultJson.length()) {
          scanResultsJson[scanResultsJson.length()] = scanResultJson;
        }

        // Get array string.
        scanResultsString = JSON.stringify(scanResultsJson);

        // Create result json.
        scanResultJson["ssid"] = WiFi.SSID(resultIndex);
        scanResultJson["rssi"] = WiFi.RSSI(resultIndex);
        scanResultJson["sec"] = WiFi.encryptionType(resultIndex);

        // Get result string.
        scanResultString = JSON.stringify(scanResultJson);

        // Increment the result index.
        resultIndex++;
      } while (((scanResultsString.length() + scanResultString.length())
          < maxPacketLen)
          && (resultIndex <= resultCount));

      delete[] scanResultJson;
      delete[] scanResultsJson;

      // Set to the correct char.
      switch (charApListPartIndex++) {
        case 1: {
          bleServiceWifi_->charApListPart1_
                         ->setValue(scanResultsString.c_str());
          log_i("charApListPart1_: %s",
                scanResultsString.c_str());
        }
          break;
        case 2: {
          bleServiceWifi_->charApListPart2_
                         ->setValue(scanResultsString.c_str());
          log_i("charApListPart2_: %s",
                scanResultsString.c_str());

        }
          break;
        case 3: {
          bleServiceWifi_->charApListPart3_
                         ->setValue(scanResultsString.c_str());
          log_i("charApListPart3_: %s",
                scanResultsString.c_str());
        }
          break;
        case 4: {
          bleServiceWifi_->charApListPart4_
                         ->setValue(scanResultsString.c_str());
          log_i("charApListPart4_: %s",
                scanResultsString.c_str());
        }
          break;
        case 5: {
          bleServiceWifi_->charApListPart5_
                         ->setValue(scanResultsString.c_str());
          log_i("charApListPart5_: %s",
                scanResultsString.c_str());
        }
          break;
        default: {
          log_w("Too many wifi networks found. The following network/s will be truncated from the list: %s",
                scanResultsString.c_str());
        }
          break;
      }
    }
  }
  else {
    log_i("WiFi scan returned no results.");
  }
  // Set the scanning state.
  setScanState(ScanState::SCANNED);
}

/// Method run by the FreeRTOS task.
void CoBmecWifi::task(void *coBmecWifiRef) {
  // Init the backoff index.
  static int backoffIndex = 0;
  // Init the last connect time.
  static unsigned long lastConnectTMs = 0;
  // Init backoff delay.
  static uint32_t backoffMs = 0;


  // Collect the calling instance.
  auto *coBmecWifi = static_cast<CoBmecWifi *>(coBmecWifiRef);

  WiFiEvent_t event;

  for (;;) {

    vTaskDelay(WIFI_IDLE_TIME_MS / portTICK_PERIOD_MS);

    if (xQueueReceive(coBmecWifi->wifiEventQueue_,
                      &event,
                      0)) {
      coBmecWifi->onWifiEvent(event);
    }

    switch (coBmecWifi->apState_) {
      case ApState::UNDEFINED:
      case ApState::ERROR:
      case ApState::DISCONNECTING:break;
      case ApState::DISCONNECTED: {
        if (coBmecWifi->scanState_ != ScanState::SCANNING) {
          // Return if delay has not expired.
          if ((millis() - lastConnectTMs) > backoffMs) {
            // Set the last connection attempt time.
            lastConnectTMs = millis();

            // Start the connection attempt.
            coBmecWifi->wifiConnect();

            // Compute the backoff delay.
            backoffMs = (uint32_t) min(int(pow(2,
                                               backoffIndex++)) * 1000
                                           + int(random(1000)),
                                       CONNECT_MAX_BACKOFF_MS);

            // Log the backoff.
            log_i("If WiFi connection fails next attempt will be in %lu ms",
                  backoffMs);
          }
        }
      }
        break;
      case ApState::CONNECTING:break;
      case ApState::CONNECTED: {
        // Reset the backoff index.
        backoffIndex = 0;
        // Reset the last ping time.
        lastConnectTMs = 0;
        // Reset backoff delay.
        backoffMs = 0;

        // Start the ping attempt.
        coBmecWifi->internetPing();
      }
        break;
      case ApState::PINGING:
      case ApState::PINGED:break;
    }
  }
}

/// If PINGED then return to CONNECTED to force ping test.
void CoBmecWifi::checkInternet() {
  if(apState_ == ApState::PINGED){
    log_i("Internet check started...");
    setApState(ApState::CONNECTED);
  } else {
    log_w("Internet check requested but the AP is busy with a connection attempt or not connected.");
  }
}

/// @}