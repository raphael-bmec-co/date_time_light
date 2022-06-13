///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_config_service.h
/// @brief BLE config service.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

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
/// BLE state service class.
class BleServiceConfig {

  class BleServiceConfig1 {
   public:
    explicit BleServiceConfig1(BLEServer *server);

    // Service
    BLEService *bleService_{};

    // Characteristics for the service.
    BLECharacteristic *newDataChar_{};

    BLECharacteristic *firmwareVersionChar_{};
    BLECharacteristic *customerNameChar_{};
    BLECharacteristic *customerKeyChar_{};
    BLECharacteristic *siteNameChar_{};
    BLECharacteristic *siteKeyChar_{};
    BLECharacteristic *lineNameChar_{};
    BLECharacteristic *pingTMsChar_{};
    BLECharacteristic *enabledChar_{};
    BLECharacteristic *nozzleCountChar_{};
    BLECharacteristic *enabledInputBypassedChar_{};
    BLECharacteristic *errorAutoResetChar_{};
    BLECharacteristic *errorAutoResetDelaySChar_{};
    BLECharacteristic *warningAlertDelaySChar_{};
    BLECharacteristic *warningAlertRepeatDelaySChar_{};
    BLECharacteristic *errorAlertDelaySChar_{};
    BLECharacteristic *errorAlertRepeatDelaySChar_{};
    BLECharacteristic *reactor1EnabledChar_{};
    BLECharacteristic *reactor2EnabledChar_{};
    BLECharacteristic *reactor3EnabledChar_{};
    BLECharacteristic *reactor4EnabledChar_{};
    BLECharacteristic *currentSetPointMaChar_{};
    BLECharacteristic *currentWarningPercentChar_{};
    BLECharacteristic *currentErrorPercentChar_{};
    BLECharacteristic *ppmSetPointChar_{};
    BLECharacteristic *ppmWarningPercentChar_{};
    BLECharacteristic *ppmErrorPercentChar_{};

   private:

  };

  class BleServiceConfig2 {
   public:
    explicit BleServiceConfig2(BLEServer *server);

    // Service
    BLEService *bleService_{};

    // Characteristics for the service.
    BLECharacteristic *airFlowPerNozzleSetPointMlpmChar_{};
    BLECharacteristic *airFlowWarningPercentChar_{};
    BLECharacteristic *airFlowErrorPercentChar_{};
    BLECharacteristic *waterFlowPerNozzleSetPointMlpmChar_{};
    BLECharacteristic *waterFlowWarningPercentChar_{};
    BLECharacteristic *waterFlowErrorPercentChar_{};

   private:

  };

 public:

  explicit BleServiceConfig(BLEServer *server);

  BleServiceConfig1 *bleServiceConfig1_{};
  BleServiceConfig2 *bleServiceConfig2_{};

  void start() const;


 private:

};

/// @}