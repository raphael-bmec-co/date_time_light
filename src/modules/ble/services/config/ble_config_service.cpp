///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_config_service.cpp
/// @brief BLE config service.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLE2902.h>
#include "ble_config_service.h"

//*********************************************************************
// #defines
//*********************************************************************
static uint8_t BLE_SERVICE_1_HANDLES = 30 * 2;
static uint8_t BLE_SERVICE_2_HANDLES = 7 * 2;
static const char *BLE_SERVICE_1_BASE_UUID = "-0003-0000-0000-681ff943633b";
static const char *BLE_SERVICE_2_BASE_UUID = "-0004-0000-0000-681ff943633b";

//*********************************************************************
// #constructors.
//*********************************************************************
/// Creates the service on the server.
/// @param server

BleServiceConfig::BleServiceConfig(BLEServer *server){
  bleServiceConfig1_ = new BleServiceConfig1(server);
  bleServiceConfig2_ = new BleServiceConfig2(server);
}

/// Starts the child services.
void BleServiceConfig::start() const {

  bleServiceConfig1_->bleService_->start();
  bleServiceConfig2_->bleService_->start();

};

BleServiceConfig::BleServiceConfig1::BleServiceConfig1(BLEServer *server) {

  // Init the iterator.
  int serviceIndex = 10;

  // Create service.
  bleService_ = server->createService(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLE_SERVICE_1_HANDLES);

  // Add the characteristics.
  bleService_->addCharacteristic(newDataChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY));

  bleService_->addCharacteristic(firmwareVersionChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(customerNameChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(customerKeyChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(siteNameChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(siteKeyChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(lineNameChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(pingTMsChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(enabledChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(nozzleCountChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(enabledInputBypassedChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(errorAutoResetChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(errorAutoResetDelaySChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(warningAlertDelaySChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(warningAlertRepeatDelaySChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(errorAlertDelaySChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(errorAlertRepeatDelaySChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(reactor1EnabledChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(reactor2EnabledChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(reactor3EnabledChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(reactor4EnabledChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(currentSetPointMaChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(currentWarningPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(currentErrorPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(ppmSetPointChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(ppmWarningPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(ppmErrorPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  
  // Add the descriptors.
  newDataChar_->addDescriptor(new BLE2902());
}

BleServiceConfig::BleServiceConfig2::BleServiceConfig2(BLEServer *server) {

  // Init the iterator.
  int serviceIndex = 10;

  // Create service.
  bleService_ = server->createService(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLE_SERVICE_2_HANDLES);

  // Add the characteristics.
  bleService_->addCharacteristic(airFlowPerNozzleSetPointMlpmChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(airFlowWarningPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(airFlowErrorPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(waterFlowPerNozzleSetPointMlpmChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(waterFlowWarningPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
  bleService_->addCharacteristic(waterFlowErrorPercentChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE));
}

/// @}