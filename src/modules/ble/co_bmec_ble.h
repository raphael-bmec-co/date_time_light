///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @defgroup ble ble
/// @brief BLE module.
/// @{

/// @file co_bmec_ble.h
/// @brief BLE module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

#pragma once

//*********************************************************************
// #includes.
//*********************************************************************
#include "Arduino.h"
#include "services/wifi/ble_wifi_service.h"

//*********************************************************************
// #defines
//*********************************************************************

//*********************************************************************
// #forward declarations
//*********************************************************************

//*********************************************************************
// class declarations
//*********************************************************************
/// BLE server class.
class CoBmecBle {
 public:

  static void *ref_;

  static BLEServer *bleServer_;
  static BleServiceWifi *bleServiceWifi_;

  static void init(void *ref, void (*onConnectionStateChanged)(bool));
  static void startAdvertising(const char* advertisingName);

 private:

  static void (*onConnectionStateChanged_)(bool connected);
};

/// @}