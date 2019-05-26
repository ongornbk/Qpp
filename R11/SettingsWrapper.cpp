#include "pch.h"
#include "SettingsWrapper.h"
#include "Settings.h"

namespace Wrapper
{

	std::string __GetSetting(const std::string key)
	{
		return GetSetting(key).c_str();
	}

}