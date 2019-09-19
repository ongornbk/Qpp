#include "pch.h"
#include "SystemHelper.h"

SystemInfo SystemHelper::GetSystemVersion()
{
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;

	*(FARPROC*)& RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		return SystemInfo(osInfo);
	}
	else throw std::exception("Error! cannot find ntdll function -> RtlGetVersion");
	return SystemInfo();
}