///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @defgroup wifi wifi
/// @brief Wifi module.
/// @{

/// @file co_bmec_wifi.h
/// @brief Wifi module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

#pragma once

//*********************************************************************
// #includes.
//*********************************************************************
#include <esp_wifi_types.h>
#include <WiFiGeneric.h>
#include "modules/ble/services/wifi/ble_wifi_service.h"
#include <Preferences.h>


//*********************************************************************
// defines.
//*********************************************************************
#define DEFAULT_WIFI_SSID                  "43 Arnold Street"
#define DEFAULT_WIFI_PASSWORD              "891025255"
#define CO_BMEC_WIFI_EVENT_QUEUE_LENGTH    10
#define WIFI_EVENT_QUEUE_IDLE_TIME_MS      5

//*********************************************************************
// forward declarations.
//*********************************************************************

//*********************************************************************
// class declarations.
//*********************************************************************
class CoBmecWifi : public BLECharacteristicCallbacks {
 public:

  enum class ApCommand {
    UNDEFINED, DISCONNECT, CONNECT, PING,
  };

  enum class ApState {
    UNDEFINED, ERROR, DISCONNECTING, DISCONNECTED, CONNECTING, CONNECTED, PINGING, PINGED,
  };

  enum class ScanCommand {
    UNDEFINED, SCAN,
  };

  enum class ScanState {
    UNDEFINED, ERROR, SCANNING, SCANNED,
  };

  struct Config {
    String ssid_;
    bool ssidUpdated_ = false;
    wifi_auth_mode_t security_ = WIFI_AUTH_OPEN;
    bool securityUpdated_ = false;
    String identity_;
    bool identityUpdated_ = false;
    String username_;
    bool usernameUpdated_ = false;
    String password_;
    bool passwordUpdated_ = false;
  };

  CoBmecWifi(void *ref,
             int core,
             int priority,
             SemaphoreHandle_t flashMutex,
             BleServiceWifi *bleServiceWifi,
             void (*apStateCallback)(CoBmecWifi *, ApState));

  void *ref_;

  Config *config_ = new Config();

  static long getRssiDbm();

  ApState getState() {
    return apState_;
  };

  void init();

  void checkInternet();

 private:

  int core_;  ///< Core on which to run the FreeRTOS task.
  int priority_;  ///< Priority at which to run the FreeRTOS task.
  SemaphoreHandle_t flashMutex_;

  Preferences preferences_{};
  BleServiceWifi *bleServiceWifi_;

  void (*apStateCallback_)(CoBmecWifi *, ApState);

  ApState apState_ = ApState::DISCONNECTED;
  ScanState scanState_ = ScanState::UNDEFINED;

  QueueHandle_t wifiEventQueue_ = xQueueCreate(CO_BMEC_WIFI_EVENT_QUEUE_LENGTH, sizeof(WiFiEvent_t));

  void setApState(ApState apState);

  __unused void setApError(const String &errorMessage);

  void setScanState(ScanState scanState);

  void setScanError(const String &errorMessage);

  void onWifiEvent(WiFiEvent_t event);

  void loadConfig();

  void setConfig();

  void saveConfig();

  void deleteConfig();

  void wifiDisconnect();

  void wifiConnect();

  void internetPing();

  void onScanDone();

  // Overrides.
  void onWrite(BLECharacteristic *characteristic) override;

  [[noreturn]] static void task(void *coBmecWifiRef);

};

/// @}
