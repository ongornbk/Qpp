#pragma once

struct SystemInfo
{
	SystemInfo()
	{
		major = 0u;
		minor = 0u;
		platform = 0u;
		build = 0u;
	}

	SystemInfo(const OSVERSIONINFOEXW& info)
	{
		major = info.dwMajorVersion;
		minor = info.dwMinorVersion;
		platform = info.dwPlatformId;
		build = info.dwBuildNumber;
	}


	uint32 major;
	uint32 minor;
	uint32 platform;
	uint32 build;
};

class SystemHelper
{
public:
	static SystemInfo GetSystemVersion();
};
