/// @ingroup date_time_light
/// @{

/// @file date_time_light.cpp
/// @brief main DateTimeLight module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2020 Raphael Smith. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include <esp_adc_cal.h>
#include <BLECharacteristic.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>
#include <modules/ble/co_bmec_ble.h>
#include <modules/ota/co_bmec_ota.h>
#include "date_time_light.h"

//*********************************************************************
// defines.
//*********************************************************************


//*********************************************************************
// definitions.
//*********************************************************************

//*********************************************************************
// Constants.
//*********************************************************************

/// FREE RTOS.
static const uint32_t CORE_0_TASK_STACK_SIZE = 5000;  // Used 2152 -> 2500.
static const uint32_t CORE_1_TASK_STACK_SIZE =
    12000;  /// Used 2644 -> 12000. This is used by OTA a large margin of error is required.

static const uint32_t LOOP_0_IDLE_TIME_MS = 100;
static const uint32_t LOOP_1_IDLE_TIME_MS = 100;

/// LED STRIP.
static int16_t LED_PIN = 13;
static uint16_t DT_HOURS_PER_DAY = 24;
static uint16_t DT_DAYS_PER_WEEK = 7;
static uint16_t DT_LED_COUNT = DT_HOURS_PER_DAY * DT_DAYS_PER_WEEK;
static uint16_t WIFI_LED = 0;
static uint16_t BLE_LED = 1;

/// NTP
const char *NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 2 * 60 * 60;

/// Stars low and high priority loops.
void DateTimeLight::init() {

  log_i("DateTimeLight init.");

  // Start low priority loop.
  (void) xTaskCreatePinnedToCore(
      [](void *dateTimeLightRef) {
        auto *dateTimeLight = static_cast<DateTimeLight *>(dateTimeLightRef);
        dateTimeLight->core0Loop();
      }, // Function to implement the task
      "core_0_loop", // Name of the task
      CORE_0_TASK_STACK_SIZE,  // Stack size in words.
      this,  // Task input parameter
      NOMINAL_PRIORITY,  // Priority of the task
      nullptr,  // Task handle.
      LOW_PRIORITY_CORE); // Core where the task should run
  // Start high priority loop.
  (void) xTaskCreatePinnedToCore(
      [](void *dateTimeLightRef) {
        auto *dateTimeLight = static_cast<DateTimeLight *>(dateTimeLightRef);
        dateTimeLight->core1Loop();
      }, // Function to implement the task
      "core_1_loop", // Name of the task
      CORE_1_TASK_STACK_SIZE,  // Stack size in words.
      this,  // Task input parameter
      NOMINAL_PRIORITY,  // Priority of the task
      nullptr,  // Task handle.
      HIGH_PRIORITY_CORE); // Core where the task should run

  log_i("DateTimeLight initialised.");
}

void DateTimeLight::core0Loop() {
  log_i("cor01Loop started on core: %u", xPortGetCoreID());

  // Init the BLE module.
  CoBmecBle::init(
      this, bleConnectionStateCallback);

  // Instantiate and init the Wi-Fi module.
  coBmecWifi_ = new CoBmecWifi(
      this,
      LOW_PRIORITY_CORE,
      NOMINAL_PRIORITY,
      flashMutex_,
      CoBmecBle::bleServiceWifi_,
      &apStateCallback);

  // Set the callback reference.
  coBmecWifi_->ref_ = this;

  // Init the Wi-Fi module.
  coBmecWifi_->init();

  // Set the init state of the core.
  core0Inited_ = true;

  for (;;) {

    vTaskDelay(LOOP_0_IDLE_TIME_MS / portTICK_PERIOD_MS);

    // FEATURE replace this by setting the watchdog time. Cannot be done without recompiling the arduino core.
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
  }
}

void DateTimeLight::core1Loop() {
  log_i("core1Loop started on core: %u", xPortGetCoreID());

  /// Init the strip.
  strip_ = new Adafruit_NeoPixel(
      DT_LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  strip_->begin();  // Init.
  strip_->show();  // Clear.

  // Set the core initialised.
  core1Inited_ = true;

  int rippleIndex1 = 0;
  int rippleIndex2 = 1;
  int rippleIndex3 = 2;

  for (;;) {

    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;

    vTaskDelay(LOOP_1_IDLE_TIME_MS / portTICK_PERIOD_MS);

    strip_->show();

    // Time sync failed.
    if (!getLocalTime(&timeInfo_)) {
      log_w("Waiting on time sync...");
      continue;
    }

    /// Fill with rainbow.
    strip_->rainbow(0, 1, 255,50);

    /// Set the hours pixels.
    strip_->fill(
        Adafruit_NeoPixel::Color(0,0,0),
                (timeInfo_.tm_wday) * DT_HOURS_PER_DAY + timeInfo_.tm_hour);

    /// Set the ripple that moves around.
    strip_->setPixelColor(
        rippleIndex1 < DT_LED_COUNT ? rippleIndex1++ : rippleIndex1 = 0,
        0,
        0,
        5);

    strip_->setPixelColor(
        rippleIndex2 < DT_LED_COUNT ? rippleIndex2++ : rippleIndex2 = 0,
        0,
        0,
        25);

    strip_->setPixelColor(
        rippleIndex3 < DT_LED_COUNT ? rippleIndex3++ : rippleIndex3 = 0,
        0,
        0,
        50);
  }
}

void DateTimeLight::bleConnectionStateCallback(bool connected) {
  // FEATURE implement BLE.
  //  auto *dateTimeLight = static_cast<DateTimeLight *>(CoBmecBle::ref_);

  if (connected) {
    log_i("BLE device connected");
    {
      //      dateTimeLight->strip_->setPixelColor(
      //          BLE_LED, 0, 0, 255);
    }
  } else {
    log_i("BLE device disconnected");
    {
      //      dateTimeLight->strip_->setPixelColor(
      //          BLE_LED, 0, 0, 0);
    }
  }
}

void DateTimeLight::apStateCallback(
    CoBmecWifi *coBmecWifi, CoBmecWifi::ApState apState
) {

  auto *dateTimeLight = static_cast<DateTimeLight *>(coBmecWifi->ref_);

  switch (apState) {
    case CoBmecWifi::ApState::UNDEFINED:
    case CoBmecWifi::ApState::ERROR:
    case CoBmecWifi::ApState::DISCONNECTING:
    case CoBmecWifi::ApState::DISCONNECTED: {
      dateTimeLight->strip_->setPixelColor(
          WIFI_LED, 255, 0, 0);
      dateTimeLight->strip_->show();
    }
      break;
    case CoBmecWifi::ApState::CONNECTING:
    case CoBmecWifi::ApState::CONNECTED: {
      dateTimeLight->strip_->setPixelColor(
          WIFI_LED, 255, 150, 0);
      dateTimeLight->strip_->show();
    }
      break;
    case CoBmecWifi::ApState::PINGING:break;
    case CoBmecWifi::ApState::PINGED: {

      /// Config the time.
      configTime(GMT_OFFSET_SEC, 0, NTP_SERVER);

      dateTimeLight->strip_->setPixelColor(
          WIFI_LED, 0, 5, 0);
      dateTimeLight->strip_->show();
    }
      break;
  }
}


/// @}