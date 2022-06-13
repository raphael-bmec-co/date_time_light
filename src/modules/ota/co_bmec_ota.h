#include <sys/cdefs.h>
#include <string>
#include <functional>
///   ___   _ _   ___   ___
///  |___) | | | |___  |
///  |___) |   | |___  |___
///

/// @defgroup ota ota
/// @brief Over the air firmware update module.
/// @{

/// @file co_bmec_ota.h
/// @brief Over the air firmware update module.
/// @author Raphael Smith.
/// @copyright Copyright (c) 2021 BMEC Technologies. All rights reserved.

#pragma once
//*********************************************************************
// #includes.
//*********************************************************************

//*********************************************************************
// defines.
//*********************************************************************


//*********************************************************************
// forward declarations.
//*********************************************************************

//*********************************************************************
// class declarations.
//*********************************************************************
class CoBmecOta {
 public:
  static void ota(const std::string& url, const std::function<void()>& beforeRestart = [](){});

 private:

};

/// @}