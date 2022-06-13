///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @defgroup date_time_light date_time_light
/// @brief Controller for a NeoPixel LED strip that shows the time and day of the week.
///  @{

/// @file date_time_light.h
/// @brief main DateTimeLight module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2020 Raphael Smith. All rights reserved.

#pragma once

//*********************************************************************
// #includes.
//*********************************************************************
#include "Arduino.h"
#include "Wire.h"
#include "../../../.pio/libdeps/esp32dev/Adafruit NeoPixel/Adafruit_NeoPixel.h"
#include <string>
#include <algorithm>
#include <BLECharacteristic.h>
#include <modules/wifi/co_bmec_wifi.h>

//*********************************************************************
// defines.
//*********************************************************************

// FREERTOS.
#define LOW_PRIORITY_CORE           0
#define HIGH_PRIORITY_CORE          1
#define NOMINAL_PRIORITY            4

// ESP32 pins.

//*********************************************************************
// Constants.
//*********************************************************************

//*********************************************************************
// forward declarations.
//*********************************************************************

//*********************************************************************
// class declarations.
//*********************************************************************

class DateTimeLight {
 public:
  enum class State {
    CONNECTING,
    RUNNING,
  };

  void init();

 private:

  Preferences preferences_;

  /// Mutexes
  SemaphoreHandle_t flashMutex_ = xSemaphoreCreateMutex();

  /// Communication.
  CoBmecWifi *coBmecWifi_{};

  bool core0Inited_ = false;
  bool core1Inited_ = false;

  /// LED Strip.
  Adafruit_NeoPixel* strip_{};

  /// NTP.
  struct tm timeInfo_{};

  /// Core loops.
  [[noreturn]] void core0Loop();

  [[noreturn]] void core1Loop();

  /// Module callbacks.

  static void bleConnectionStateCallback(bool connected);

  static void apStateCallback(CoBmecWifi *, CoBmecWifi::ApState apState);

};



/// @}
