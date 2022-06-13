///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_ciotc_service.h
/// @brief BLE ciotc services for the configuration of the ciotc module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2020 BMEC Technologies. All rights reserved.

#pragma once

//*********************************************************************
// #includes.
//*********************************************************************

//*********************************************************************
// #defines
//*********************************************************************

//*********************************************************************
// #forward declarations
//*********************************************************************

//*********************************************************************
// class declarations
//*********************************************************************
/// BLE ciotc service class.
class BleServiceCiotc {
  public:
    explicit BleServiceCiotc(BLEServer *server);

    // Service
    BLEService *bleService_{};

    // Characteristics for the service.
    // FEATURE add the ciotc State and implement in the module.
    BLECharacteristic *charDeviceId_{};
    BLECharacteristic *charPrivateKey_{};
  private:

};

/// @}