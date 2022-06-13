///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file co_bmec_ble.c
/// @brief BLE module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include <BLEDevice.h>
#include "co_bmec_ble.h"

//*********************************************************************
// defines.
//*********************************************************************

//*********************************************************************
// constructors.
//*********************************************************************

//*********************************************************************
// definitions.
//*********************************************************************
void *CoBmecBle::ref_;

void (*CoBmecBle::onConnectionStateChanged_)(bool connected);

BLEServer *CoBmecBle::bleServer_;
BleServiceWifi *CoBmecBle::bleServiceWifi_;

//*********************************************************************
// implementations.
//*********************************************************************

/// Initialises the ble module.
/// @param ref global reference for callbacks.
/// @param onConnectionStateChanged callback for connection state changes.
void CoBmecBle::init(void *ref, void (*onConnectionStateChanged)(bool)) {
  log_i("BLE init.");

  // Set the reference.
  ref_ = ref;

  // Set the callback.
  onConnectionStateChanged_ = onConnectionStateChanged;

  // Frees some extra stack.
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

  // Create the BLE Device.
  BLEDevice::init("");

  // Set the power level to max.
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  // Create the BLE Server.
  bleServer_ = BLEDevice::createServer();

  // Create the callback class
  class BmeBleServerCallbacks : public BLEServerCallbacks {

   private:

    void onConnect(BLEServer *pServer) override {
      onConnectionStateChanged_(true);
    }

    void onDisconnect(BLEServer *pServer) override {
      pServer->getAdvertising()->start();
      onConnectionStateChanged_(false);
    }

  };

  // Set the callback.
  bleServer_->setCallbacks(new BmeBleServerCallbacks());

  // Create services.
  bleServiceWifi_ = new BleServiceWifi(bleServer_);

  // Start services.
  bleServiceWifi_->bleService_->start();

  log_i("BLE initialised.");
}

/// Sets the device name and starts advertising.
/// @param advertisingName advertising name for the device.
void CoBmecBle::startAdvertising(const char *advertisingName) {

  log_i("Starting advertising as %s", advertisingName);

  // Set the device name.
  esp_ble_gap_set_device_name(advertisingName);

  // Start advertising
  bleServer_->getAdvertising()->start();

}

/// @}