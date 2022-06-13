///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_ciotc_service.cpp
/// @brief BLE ciotc services for the configuration of the ciotc module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2020 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include "Arduino.h"
#include <BLEDevice.h>
#include "ble_ciotc_service.h"

//*********************************************************************
// #defines
//*********************************************************************
#define BLE_SERVICE_CIOTC_HANDLES       6
#define BLE_SERVICE_CIOTC_BASE_UUID     "-0002-0000-0000-681ff943633b"

//*********************************************************************
// #constructors.
//*********************************************************************
/// Creates the service on the server.
/// @param server
BleServiceCiotc::BleServiceCiotc(BLEServer *server) {

    int uuidIndex = 10;

    // Create service.
    bleService_ = server->createService(BLEUUID(("000000" + String(uuidIndex++) + BLE_SERVICE_CIOTC_BASE_UUID).c_str()), BLE_SERVICE_CIOTC_HANDLES);

    // Add the characteristics.
    bleService_->addCharacteristic(charDeviceId_ = new BLECharacteristic(BLEUUID(("000000" + String(uuidIndex++) + BLE_SERVICE_CIOTC_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ));
    bleService_->addCharacteristic(charPrivateKey_ = new BLECharacteristic(BLEUUID(("000000" + String(uuidIndex++) +  BLE_SERVICE_CIOTC_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_WRITE));
}

/// @}