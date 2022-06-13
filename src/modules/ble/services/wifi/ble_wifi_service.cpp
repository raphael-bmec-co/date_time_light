///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ble ble
/// @{

/// @file ble_wifi_service.c
/// @brief BLE wifi services for the configuration of the wifi.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include <BLEDevice.h>
#include <BLE2902.h>
#include <BLE2904.h>
#include "ble_wifi_service.h"

//*********************************************************************
// #defines
//*********************************************************************
#define BLE_SERVICE_WIFI_HANDLES 70

//*********************************************************************
// #constructors.
//*********************************************************************
/// Creates the startService on the server.
/// @param server
BleServiceWifi::BleServiceWifi(BLEServer *server) {

    // Create startService.
    bleService_ = server->createService(BLEUUID(UUID), BLE_SERVICE_WIFI_HANDLES);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApCommand_);
    charApCommand_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApState_);
    charApState_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApError_);
    charApError_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charScanCommand_);
    charScanCommand_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charScanState_);
    charScanState_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charScanError_);
    charScanError_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charSsid_);
    charSsid_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharSsidDescBle2904 = new BLE2904();
    pCharSsidDescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charSsid_->addDescriptor(pCharSsidDescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charSecurity_);
    charSecurity_->addDescriptor(new BLE2902());

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charIdentity_);
    charIdentity_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharIdentityDescBle2904 = new BLE2904();
    pCharIdentityDescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charIdentity_->addDescriptor(pCharIdentityDescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charUsername_);
    charUsername_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharUsernameDescBle2904 = new BLE2904();
    pCharUsernameDescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charUsername_->addDescriptor(pCharUsernameDescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charPassword_);
//    charPassword_->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);  Feature enable to improve security.
    charPassword_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharPasswordDescBle2904 = new BLE2904();
    pCharPasswordDescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charPassword_->addDescriptor(pCharPasswordDescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApListPart1_);
    charApListPart1_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharApListPart1DescBle2904 = new BLE2904();
    pCharApListPart1DescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charApListPart1_->addDescriptor(pCharApListPart1DescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApListPart2_);
    charApListPart2_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharApListPart2DescBle2904 = new BLE2904();
    pCharApListPart2DescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charApListPart2_->addDescriptor(pCharApListPart2DescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApListPart3_);
    charApListPart3_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharApListPart3DescBle2904 = new BLE2904();
    pCharApListPart3DescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charApListPart3_->addDescriptor(pCharApListPart3DescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApListPart4_);
    charApListPart4_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharApListPart4DescBle2904 = new BLE2904();
    pCharApListPart4DescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charApListPart4_->addDescriptor(pCharApListPart4DescBle2904);

    // -----------------------------------------------------------------------------------------------------------------
    bleService_->addCharacteristic(charApListPart5_);
    charApListPart5_->addDescriptor(new BLE2902());

    // Add format descriptor.
    auto *pCharApListPart5DescBle2904 = new BLE2904();
    pCharApListPart5DescBle2904->setFormat(BLE2904::FORMAT_UTF8);
    charApListPart5_->addDescriptor(pCharApListPart5DescBle2904);
}

/// @}