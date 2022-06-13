///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_status_service.cpp
/// @brief BLE status service.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include "Arduino.h"
#include "ble_status_service.h"

//*********************************************************************
// #defines
//*********************************************************************
static uint8_t BLE_SERVICE_1_HANDLES = 26 * 2;
static uint8_t BLE_SERVICE_2_HANDLES = 19 * 2;
static const char *BLE_SERVICE_1_BASE_UUID = "-0005-0000-0000-681ff943633b";
static const char *BLE_SERVICE_2_BASE_UUID = "-0006-0000-0000-681ff943633b";

//*********************************************************************
// #constructors.
//*********************************************************************
/// Creates the service on the server.
/// @param server
BleServiceStatus::BleServiceStatus(BLEServer *server) {
  bleServiceStatus1_ = new BleServiceStatus1(server);
  bleServiceStatus2_ = new BleServiceStatus2(server);
}

/// Starts the child services.
void BleServiceStatus::start() const {

  bleServiceStatus1_->bleService_->start();
  bleServiceStatus2_->bleService_->start();

};

BleServiceStatus::BleServiceStatus1::BleServiceStatus1(BLEServer *server) {

  // Init the iterator.
  int serviceIndex = 10;

  // Create service.
  bleService_ = server->createService(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLE_SERVICE_1_HANDLES);

  // Add the characteristics.
  bleService_->addCharacteristic(newDataChar_ = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY));

  bleService_->addCharacteristic(runtimeMChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(stateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(enableSwitchEnabledChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(enabledInputEnabledChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneSafetySensorSafeChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(waterFlowMlpmChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(waterPressureMbarChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(waterAoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(waterErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(coolingPressureMbarChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(coolingTempCChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(coolingAtmosphericPressureMbarChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(coolingAoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(coolingErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(airFlowSmlpmChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(airAoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(airErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneOzonePpmChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneOzoneAiChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneScrubbedAiChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneTempDegCChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozonePressureMbarChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(ozoneErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_1_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));


  // Add the descriptors.
  newDataChar_->addDescriptor(new BLE2902());
}

BleServiceStatus::BleServiceStatus2::BleServiceStatus2(BLEServer *server) {

  // Init the iterator.
  int serviceIndex = 10;

  // Create service.
  bleService_ = server->createService(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLE_SERVICE_2_HANDLES);

  // Add the characteristics.
  bleService_->addCharacteristic(reactor1CurrentMaChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor1AoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor1ErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor2CurrentMaChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor2AoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor2ErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor3CurrentMaChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor3AoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor3ErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor4CurrentMaChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor4AoChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(reactor4ErrorStateChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(batteryOkayChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(batteryVoltageChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(psuOkayChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(psuVoltageChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(warningAlertChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
  bleService_->addCharacteristic(errorAlertChar_  = new BLECharacteristic(BLEUUID(("000000" + String(serviceIndex++) + BLE_SERVICE_2_BASE_UUID).c_str()), BLECharacteristic::PROPERTY_READ));
}

/// @}