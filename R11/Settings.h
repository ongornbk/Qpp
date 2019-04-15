#pragma once
#include "String.h"

int32 InitializeSettings(const char* filename);

void PushSetting(const std::string key, const std::string value);

String GetSetting(const std::string key);