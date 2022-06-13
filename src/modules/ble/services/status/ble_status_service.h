///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_status_service.h
/// @brief BLE status service.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

#pragma once

#include <BLEDevice.h>
#include <BLE2902.h>

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
/// BLE state service class.
class BleServiceStatus {
  class BleServiceStatus1 {
   public:
    explicit BleServiceStatus1(BLEServer *server);

    // Service
    BLEService *bleService_{};

    // Characteristics for the service.
    BLECharacteristic *newDataChar_{};

    BLECharacteristic *runtimeMChar_{};
    BLECharacteristic *stateChar_{};
    BLECharacteristic *enableSwitchEnabledChar_{};
    BLECharacteristic *enabledInputEnabledChar_{};
    BLECharacteristic *ozoneSafetySensorSafeChar_{};

    BLECharacteristic *waterFlowMlpmChar_{};
    BLECharacteristic *waterPressureMbarChar_{};
    BLECharacteristic *waterAoChar_{};
    BLECharacteristic *waterErrorStateChar_{};

    BLECharacteristic *coolingPressureMbarChar_{};
    BLECharacteristic *coolingTempCChar_{};
    BLECharacteristic *coolingAtmosphericPressureMbarChar_{};
    BLECharacteristic *coolingAoChar_{};
    BLECharacteristic *coolingErrorStateChar_{};

    BLECharacteristic *airFlowSmlpmChar_{};
    BLECharacteristic *airAoChar_{};
    BLECharacteristic *airErrorStateChar_{};

    BLECharacteristic *ozoneOzonePpmChar_{};
    BLECharacteristic *ozoneOzoneAiChar_{};
    BLECharacteristic *ozoneScrubbedAiChar_{};
    BLECharacteristic *ozoneTempDegCChar_{};
    BLECharacteristic *ozonePressureMbarChar_{};
    BLECharacteristic *ozoneErrorStateChar_{};

   private:

  };

  class BleServiceStatus2 {
   public:
    explicit BleServiceStatus2(BLEServer *server);

    // Service
    BLEService *bleService_{};

    // Characteristics for the service.
    BLECharacteristic *reactor1CurrentMaChar_{};
    BLECharacteristic *reactor1AoChar_{};
    BLECharacteristic *reactor1ErrorStateChar_{};

    BLECharacteristic *reactor2CurrentMaChar_{};
    BLECharacteristic *reactor2AoChar_{};
    BLECharacteristic *reactor2ErrorStateChar_{};

    BLECharacteristic *reactor3CurrentMaChar_{};
    BLECharacteristic *reactor3AoChar_{};
    BLECharacteristic *reactor3ErrorStateChar_{};

    BLECharacteristic *reactor4CurrentMaChar_{};
    BLECharacteristic *reactor4AoChar_{};
    BLECharacteristic *reactor4ErrorStateChar_{};

    BLECharacteristic *batteryOkayChar_{};
    BLECharacteristic *batteryVoltageChar_{};

    BLECharacteristic *psuOkayChar_{};
    BLECharacteristic *psuVoltageChar_{};

    BLECharacteristic *warningAlertChar_{};
    BLECharacteristic *errorAlertChar_{};

   private:

  };

 public:

  explicit BleServiceStatus(BLEServer *server);

  BleServiceStatus1 *bleServiceStatus1_{};
  BleServiceStatus2 *bleServiceStatus2_{};

  void start() const;

 private:

};

/// @}