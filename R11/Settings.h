#pragma once
#include "String.h"

int32 InitializeSettings(const char* filename);

int32 LoadSettings(const char* filename);

int32 DiscardSettings();

void PushSetting(const std::string key, const std::string value);

String GetSetting(const std::string key);