#include "pch.h"
#include "Urlmon.h"
#include "LuaManager.h"
#include "Collector.h"

typedef void(*FNPTR)();
typedef long(__stdcall * pICFUNC)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);

extern "C"
{
	pICFUNC UrlToFile = nullptr;
}



struct Urlmon
{
	Urlmon()
	{
		
		m_urlmon = LoadLibraryA("urlmon.dll");
		if(m_urlmon)
		UrlToFile = pICFUNC(GetProcAddress(m_urlmon, "URLDownloadToFileA"));
		else
		{
			MessageBox(NULL, L"Error urlmon.dll: not found!", L"Error!", NULL);
		}
	}

	HMODULE     m_urlmon;
	std::string m_utlpath;

};

namespace
{
	static lua_State* m_lua = nullptr;
	static Urlmon* m_url = nullptr;
}

static int32_t _cdecl SetUrlOutputPath(lua_State* state)
{
	if(m_url)
	m_url->m_utlpath = lua_tostring(state, 1);
	else
	{
		MessageBox(NULL, L"Error urlmon package not imported!", L"Error!", NULL);
	}
	return 0;
}

static int32_t _cdecl UrlRequestToFile(lua_State* state)
{
	if (UrlToFile)
	{
		std::string target = lua_tostring(state, 1);
		std::string t0 = "https://" + target;
		HRESULT result = UrlToFile(NULL, t0.c_str(), (m_url->m_utlpath.c_str()), 0, NULL);
		if (result == S_OK)
		{
			lua_pushboolean(state,true);
		}

		else
		{
			lua_pushboolean(state,false);
		}
	}
	else
	{
		lua_pushboolean(state, false);
	}
	return 1;
}

void CALL_CONV UrlmonPackageInitializer()
{
	m_url = new Urlmon();
	push_ptr(m_url);
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "SetUrlOutputPath", SetUrlOutputPath);
	lua_register(m_lua, "UrlRequestToFile", UrlRequestToFile);

}