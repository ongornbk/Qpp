#include "pch.h"
#include "System.h"
#include "LuaManager.h"
#include <stdio.h>
#include <cstdio>
#include <thread>
#include <vector>
#include <queue>
#include <list>

using namespace std::chrono;

namespace
{
	static lua_State* m_lua = nullptr;
	static std::list<float> m_cputimes;
}

extern "C"
{

	static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	static int numProcessors;
	static HANDLE self;

	void InitCPU() {
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

}

static int32_t _cdecl __Sleep(lua_State* state)
{
	uint64_t time = (uint64_t)lua_tointeger(state, 1);
	std::this_thread::sleep_for(1ns*time);
	return 0;
}

static int32_t _cdecl __Shutdown(lua_State* state)
{
	system("shutdown -s");
	return 0;
}

static int32_t _cdecl __System(lua_State* state)
{
	std::string str = lua_tostring(state, 1);
	system(str.c_str());
	return 0;
}

static int32_t _cdecl __GetEnv(lua_State* state)
{
#pragma warning(disable : 4996)
	std::string str = getenv(lua_tostring(state, 1));
	lua_pushstring(state, str.c_str());
	return 1;
}

static int32_t _cdecl __SetAttributes(lua_State* state)
{
	std::string str = lua_tostring(state, 1);
	uint64_t attribute = lua_tointeger(state, 2);
	SetFileAttributesA(str.c_str(),attribute);
	return 0;
}

static int32_t _cdecl __GetCpuUsage(lua_State* state)
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	float percent;


	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;
	m_cputimes.push_back(percent);
	if (m_cputimes.size() >= 60)
	{
		m_cputimes.pop_front();
	}

	float sum{};
	for (auto n : m_cputimes)
	{
		sum = sum + n;
	}

	if(!m_cputimes.empty())
	sum = sum / float(m_cputimes.size());

	sum = ceilf(sum * 100);

	lua_pushinteger(m_lua, sum);
	return 1;

}



void CALL_CONV SystemPackageInitializer()
{
	m_lua = LuaManager::GetInstance()->m_lua;
	InitCPU();
	lua_register(m_lua, "Sleep", __Sleep);
	lua_register(m_lua, "Shutdown", __Shutdown);
	lua_register(m_lua, "System", __System);
	lua_register(m_lua, "GetEnv", __GetEnv);
	lua_register(m_lua, "SetAttributes", __SetAttributes);
	lua_register(m_lua, "GetCpuUsage", __GetCpuUsage);
}