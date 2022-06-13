///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @ingroup ota ota
/// @{

/// @file co_bmec_ota.cpp
/// @brief Over the air firmware update module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

//*********************************************************************
// #includes.
//*********************************************************************
#include "co_bmec_ota.h"
#include <Update.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

//*********************************************************************
// defines.
//*********************************************************************


//*********************************************************************
// definitions.
//*********************************************************************

//*********************************************************************
// implementations.
//*********************************************************************

/// Starts an OTA_UPDATE attempt. Restarts on success and failure.
/// @param url URL at which the .bin file is to be found.
/// @param optional function to run before restart.
void CoBmecOta::ota(
    const std::string &url, const std::function<void()> &beforeRestart
) {

  // Create the network client.
  auto *wifiClientSecure = new WiFiClientSecure;

  // No CA set.
  wifiClientSecure->setInsecure();

  // Create the https client.
  HTTPClient https;

  // Failed to connect.
  if (!https.begin(*wifiClientSecure,
                   url.c_str())) {
    log_e("Failed to connect to: %s",
          url.c_str());
    beforeRestart();
    ESP.restart();
  }
  // Start connection and send HTTP header.
  int httpCode = https.GET();

  // Error response.
  if (httpCode <= 0) {
    log_e("Failed to download file with error: %s",
          https.errorToString(httpCode).c_str());
    beforeRestart();
    ESP.restart();
  }

  // File not found.
  if (!(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)) {
    log_e("Unexpected HTTP code: %s",
          https.errorToString(httpCode).c_str());
    beforeRestart();
    ESP.restart();
  }

  // Get the file size.
  auto fileBytes = (size_t) https.getSize();

  log_i("File found, downloading file %u bytes...",
        fileBytes);

  // Set the callback to log the progress.
  Update.onProgress([](size_t progress_bytes, size_t total_bytes) {
    log_i("Downloading... %u%%",
          progress_bytes * 100
              / total_bytes); // NOLINT(bugprone-lambda-function-name)
  });

  // Start the update.
  if (!Update.begin(fileBytes)) {
    log_e("Failed to start update with error: %s",
          Update.errorString());
    beforeRestart();
    ESP.restart();
  }

  // Write the stream to the partition.
  Update.writeStream(https.getStream());

  if (!Update.end()) {
    log_e("Update failed with error with error %s",
          Update.errorString());
    beforeRestart();
    ESP.restart();
  }

  log_w("OTA_UPDATE update complete. Restarting to new firmware...");
  beforeRestart();
  ESP.restart();
}


/// @}