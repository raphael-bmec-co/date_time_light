///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_wifi_service.h
/// @brief BLE wifi services for the configuration of the wifi.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

#pragma once

//*********************************************************************
// #includes.
//*********************************************************************
#include <BLEServer.h>

//*********************************************************************
// #defines
//*********************************************************************

//*********************************************************************
// #forward declarations
//*********************************************************************

//*********************************************************************
// class declarations
//*********************************************************************
/// BLE wifi startService class.
class BleServiceWifi {
 public:
  explicit BleServiceWifi(BLEServer *server);

  /// UUIDs for the startService
  static constexpr const char *UUID = "00000010-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_COMMAND_UUID = "00000011-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_STATE_UUID = "00000012-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_ERROR_UUID = "00000013-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_SCAN_COMMAND_UUID = "00000014-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_SCAN_STATE_UUID = "00000015-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_SCAN_ERROR_UUID = "00000016-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_SSID_UUID = "00000017-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_SECURITY_UUID = "00000018-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_IDENTITY_UUID = "00000019-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_USERNAME_UUID = "00000020-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_PASSWORD_UUID = "00000021-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_LIST_PART_1 = "00000022-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_LIST_PART_2 = "00000023-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_LIST_PART_3 = "00000024-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_LIST_PART_4 = "00000025-0001-0000-0000-681ff943633b";
  static constexpr const char *CHAR_AP_LIST_PART_5 = "00000026-0001-0000-0000-681ff943633b";

  /// Service
  BLEService *bleService_{};

  /// Characteristics for the startService.
  BLECharacteristic *charApCommand_ = new BLECharacteristic(CHAR_AP_COMMAND_UUID, BLECharacteristic::PROPERTY_WRITE);

  BLECharacteristic *charApState_ = new BLECharacteristic(CHAR_AP_STATE_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  BLECharacteristic *charApError_ = new BLECharacteristic(CHAR_AP_ERROR_UUID, BLECharacteristic::PROPERTY_READ);

  BLECharacteristic *charScanCommand_ = new BLECharacteristic(CHAR_SCAN_COMMAND_UUID, BLECharacteristic::PROPERTY_WRITE);

  BLECharacteristic *charScanState_ = new BLECharacteristic(CHAR_SCAN_STATE_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  BLECharacteristic *charScanError_ = new BLECharacteristic(CHAR_SCAN_ERROR_UUID, BLECharacteristic::PROPERTY_READ);

  BLECharacteristic *charSsid_ = new BLECharacteristic(CHAR_SSID_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *charSecurity_ = new BLECharacteristic(CHAR_SECURITY_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *charIdentity_ = new BLECharacteristic(CHAR_IDENTITY_UUID, BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *charUsername_ = new BLECharacteristic(CHAR_USERNAME_UUID, BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *charPassword_ = new BLECharacteristic(CHAR_PASSWORD_UUID, BLECharacteristic::PROPERTY_WRITE);

  BLECharacteristic *charApListPart1_ = new BLECharacteristic(CHAR_AP_LIST_PART_1, BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *charApListPart2_ = new BLECharacteristic(CHAR_AP_LIST_PART_2, BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *charApListPart3_ = new BLECharacteristic(CHAR_AP_LIST_PART_3, BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *charApListPart4_ = new BLECharacteristic(CHAR_AP_LIST_PART_4, BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *charApListPart5_ = new BLECharacteristic(CHAR_AP_LIST_PART_5, BLECharacteristic::PROPERTY_READ);

 private:

};

/// @}