#include "pch.h"
#include "LuaManager.h"
#include "Settings.h"

namespace
{
	LuaManager* m_instance = nullptr;
	static std::list<float> m_cputimes;
}

typedef void(*FNPTR)();
typedef long(__stdcall * pICFUNC)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);

extern "C"
{

	    static pICFUNC UrlToFile = nullptr;

		static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
		static int32 numProcessors;
		static HANDLE self;

		void InitCPU()
		{
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




static int32_t _cdecl _lua_import(
	struct lua_State* const state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off+1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new UnmanagedPackage(state,pckpath, pckname);

	try
	{
		m_instance->m_pcks[pckname]->initialize();

	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "DLL Error", MB_OK);
		return 0;
	}
	return 0;
}

static int32_t _cdecl _lua_importmanaged(lua_State* state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off + 1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new ManagedPackage(state, pckpath, pckname);

	try
	{
		m_instance->m_pcks[pckname]->initialize();

	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "DLL Error", MB_OK);
		return 0;
	}
	return 0;
}


static int32_t _cdecl _lua_import_as(lua_State* state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckas = lua_tostring(state, 2);

	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off+1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new UnmanagedPackage(state,pckpath, pckname,pckas);
	try
	{
		m_instance->m_pcks[pckname]->initialize();
	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "DLL Error", MB_OK);
		return 0;
	}
	return 0;
}

static int32_t _cdecl _lua_get_argument(lua_State* state)
{
	std::string argument;
	argument = m_instance->m_args.at(lua_tointeger(state, 1));
	lua_pushstring(state, argument.c_str());
	return 1;
}

static int32_t _cdecl _lua_get_argc(lua_State* state)
{
	std::string number = std::to_string(m_instance->m_numberOfArguments);
	lua_pushstring(state, number.c_str());
	return 1;
}

static int32_t _cdecl _lua_setting(lua_State* state)
{
	lua_pushstring(state, GetSetting(lua_tostring(state,1)).c_str());
	return 1;
}

static int32_t _cdecl _lua_seturloutput(lua_State* state)
{
	m_instance->GetUrlmon().m_urlpath = lua_tostring(state, 1);
	return 0;
}

static int32_t _cdecl _lua_download(lua_State* state)
{
	if (UrlToFile)
	{
		std::string target = lua_tostring(state, 1);
		std::string t0 = "https://" + target;
		HRESULT result = UrlToFile(NULL, t0.c_str(), (m_instance->GetUrlmon().m_urlpath.c_str()), 0, NULL);
		if (result == S_OK)
		{
			lua_pushboolean(state, 0);
		}

		else
		{
			lua_pushboolean(state, 1);
		}
	}
	else
	{
		lua_pushinteger(state, 2);
	}
	return 1;
}

static int32_t _cdecl _lua_sleep(lua_State* state)
{
	using namespace std::chrono;
	const uint64 time = (uint64)lua_tointeger(state, 1);
	std::this_thread::sleep_for(1ns*time);
	return 0;
}

static int32_t _cdecl _lua_getcpu(lua_State* state)
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	float percent;


	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (float)(sys.QuadPart - lastSysCPU.QuadPart) +
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

	if (!m_cputimes.empty())
		sum = sum / float(m_cputimes.size());

	sum = ceilf(sum * 100);

	lua_pushinteger(state, (lua_Integer)sum);
	return 1;

}

static int32_t _cdecl _lua_execute(lua_State* state)
{
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = lua_tostring(state, 1);
	ShExecInfo.lpParameters = lua_tostring(state, 2);
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	lua_pushboolean(state,ShellExecuteExA(&ShExecInfo));

	return 1;
}

static int32_t _cdecl _lua_malloc(lua_State* state)
{
	lua_pushinteger(state,(lua_Integer)malloc(lua_tointeger(state,1)*64));
	return 1;
}



static int32_t _cdecl _lua_free(lua_State* state)
{
	free((void*)lua_tointeger(state, 1));
	return 0;
}


static int32_t _cdecl _lua_delete(lua_State* state)
{
	delete (void*)lua_tointeger(state, 1);
	return 0;
}

static int32_t _cdecl _lua_bzero(lua_State* state)
{
	ZeroMemory((void*)lua_tointeger(state, 1), lua_tointeger(state, 2));
	return 0;
}

static int32_t _cdecl _lua_set(lua_State* state)
{
	lua_Integer* mem = (lua_Integer*)lua_tointeger(state, 1);
	mem[lua_tointeger(state, 2)] = lua_tointeger(state, 3);
	return 0;
}



static int32_t _cdecl _lua_get(lua_State* state)
{
	lua_Integer* mem = (lua_Integer*)lua_tointeger(state, 1);
	lua_pushinteger(state, mem[lua_tointeger(state, 2)]);
	return 1;
}


static int32_t _cdecl _lua_realloc(lua_State* state)
{
	lua_Integer index = lua_tointeger(state, 1);
	lua_Integer size = lua_tointeger(state, 2);

	void* data = (void*)lua_tointeger(state, 1);

	//release_ptr(index);

	index = (lua_Integer)realloc(data,(size_t)size);

	lua_pushinteger(state, index);
	return 1;
}

static int32_t _cdecl _lua_discardsettings(lua_State* state)
{
	DiscardSettings();
	return 0;
}

static int32_t _cdecl _lua_pushsetting(lua_State* state)
{
	PushSetting(lua_tostring(state, 1), lua_tostring(state, 2));
	return 0;
}

static int32_t _cdecl _lua_loadsettings(lua_State* state)
{
	lua_pushinteger(state, LoadSettings(lua_tostring(state, 1)));
	return 1;
}

Urlmon::Urlmon()
{
	std::string urlmonpath = GetSetting("DefaultLibrariesLocation").get_string() + "/urlmon.dll";

	m_urlmon = LoadLibraryA(urlmonpath.c_str());
	if (m_urlmon)
	{
		UrlToFile = pICFUNC(GetProcAddress(m_urlmon, "URLDownloadToFileA"));
		if (!UrlToFile)
		{
			MessageBoxA(NULL, "Error! cannot initialize urlmon.dll!", "Error!", NULL);
		}
	}
	else
	{
		MessageBoxA(NULL, ("Error! compatible urlmon.dll! not found! at: " + urlmonpath + "\n Some utilities may not work properly!").c_str(), "Critical Warning!", NULL);
	}
}

LuaManager::LuaManager()
{
	m_instance = this;
	m_lua = nullptr;
}

LuaManager::~LuaManager()
{
	lua_close(m_lua);
	m_lua = nullptr;

	for (auto&& obj : m_pcks)
	{
		if (obj.second)
		{
			delete obj.second;
			obj.second = nullptr;
		}
	}
	m_pcks.clear();
}

Urlmon& LuaManager::GetUrlmon()
{
	return m_urlmon;
}

bool _cdecl LuaManager::Initialize(const int argc, char* argv[])
{
	InitCPU();
	m_numberOfArguments = argc;
	m_path = argv[0];
	m_file = argv[1];

	for (int i = 0; i < (argc); i++)
		m_args.push_back(argv[i]);

	m_lua = luaL_newstate();
	OpenLibs();
	if (!m_lua)return false;
	bool result;
	lua_register(m_lua, "import", _lua_import);
	lua_register(m_lua, "import_as", _lua_import_as);
	lua_register(m_lua, "import_managed", _lua_importmanaged);
	lua_register(m_lua, "get_argument", _lua_get_argument);
	lua_register(m_lua, "get_argc", _lua_get_argc);
	lua_register(m_lua, "get_setting", _lua_setting);
	lua_register(m_lua, "set_urloutput", _lua_seturloutput);
	lua_register(m_lua, "download", _lua_download);
	lua_register(m_lua, "sleep", _lua_sleep);
	lua_register(m_lua, "get_cpu", _lua_getcpu);
	lua_register(m_lua, "execute", _lua_execute);
	lua_register(m_lua, "malloc", _lua_malloc);
	lua_register(m_lua, "free", _lua_free);
	lua_register(m_lua, "bzero", _lua_bzero);
	lua_register(m_lua, "set", _lua_set);
	lua_register(m_lua, "get", _lua_get);
	lua_register(m_lua, "realloc", _lua_realloc);
	lua_register(m_lua, "delete", _lua_delete);
	lua_register(m_lua, "push_setting", _lua_pushsetting);
	lua_register(m_lua, "load_settings", _lua_loadsettings);
	lua_register(m_lua, "discard_settings", _lua_discardsettings);

	result = luaL_loadfile(m_lua, m_file.c_str());
	if (result)
	{
		MessageBoxA(NULL, ("Couldn't load file: " + m_file).c_str(), "Lua Error!", MB_OK);
		return false;
	}
	result = lua_pcall(m_lua, 0, LUA_MULTRET, 0);
	if (result != LUA_OK) {
		MessageBoxA(NULL, ("Lua pcall error: " + std::string(lua_tostring(m_lua, 1))).c_str(), "Lua Error!", MB_OK);
		return false;
	}
	return true;
	
}

std::string _cdecl LuaManager::GetPath() noexcept
{
	return m_path;
}

LuaManager * LuaManager::GetInstance() noexcept
{
	return m_instance;
}

void _cdecl LuaManager::OpenLibs() noexcept
{
	luaL_openlibs(m_lua);
}
