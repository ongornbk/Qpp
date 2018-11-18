#include "pch.h"
#include "Urlmon.h"
#include "LuaManager.h"

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
		UrlToFile = pICFUNC(GetProcAddress(m_urlmon, "URLDownloadToFileA"));
	}

	HMODULE     m_urlmon;
	std::string m_utlpath;

};

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<Urlmon> m_url = nullptr;
}

static int32_t _cdecl SetUrlOutputPath(lua_State* state)
{
	m_url->m_utlpath = lua_tostring(state, 1);
	return 1;
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
			lua_pushstring(state, "SUCCESS");
		}

		else
		{
			lua_pushstring(state, "Failed Request");
		}
	}
	lua_pushstring(state, "Uninitialized function UrlToFile from urlmon.dll");
	return 1;
}

void CALL_CONV UrlmonPackageInitializer()
{
	m_url = std::make_unique<Urlmon>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "SetUrlOutputPath", SetUrlOutputPath);
	lua_register(m_lua, "UrlRequestToFile", UrlRequestToFile);

}