/// @mainpage date_time_light
/// @brief Controller for a NeoPixel LED strip that shows the time and day of the week.
/// @author Raphael Smith.
/// @version see RELEASE
/// @copyright Copyright (c) 2022 Raphael Smith. All rights reserved.

/// @defgroup date_time_light date_time_light
/// @brief Controller for a NeoPixel LED strip that shows the time and day of the week.
///  @{

/// @file main.cpp
/// @brief program entrypoint.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2020 Raphael Smith. All rights reserved.

//*******************************************************************
// #includes.
//*******************************************************************
#include "Arduino.h"
#include <modules/date_time_light/date_time_light.h>

//*********************************************************************
// Constants.
//*********************************************************************

//*******************************************************************
// Variables.
//*******************************************************************
DateTimeLight dateTimeLight;

//*******************************************************************
// SETUP
//*******************************************************************
void setup() {

  // Init and start the module.
  dateTimeLight.init();

  // Remove this task to save stack space.
  vTaskDelete(nullptr);
}

void loop() {

  // Remove this task to save stack space.
  vTaskDelete(nullptr);
}

/// @}
