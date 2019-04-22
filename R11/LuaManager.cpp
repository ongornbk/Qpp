#include "pch.h"
#include "LuaManager.h"

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




static int32_t _cdecl _lua_import(lua_State* state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off+1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new LuaPackage(state,m_instance->m_ptrs,pckpath, pckname);

	try
	{
		m_instance->m_pcks[pckname]->initialize();
	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "Dll Error", MB_OK);
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

	m_instance->m_pcks[pckname] = new LuaPackage(state, m_instance->m_ptrs,pckpath, pckname,pckas);
	try
	{
		m_instance->m_pcks[pckname]->initialize();
	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "Dll Error", MB_OK);
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

	if (!m_cputimes.empty())
		sum = sum / float(m_cputimes.size());

	sum = ceilf(sum * 100);

	lua_pushinteger(state, sum);
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

static int32_t _cdecl _lua_release(lua_State* state)
{
	lua_pushboolean(state, release_ptr(lua_tointeger(state, 1)));
	return 1;
}

static int32_t _cdecl _lua_malloc(lua_State* state)
{
	lua_pushinteger(state,(lua_Integer)malloc(lua_tointeger(state,1)));
	return 1;
}

static int32_t _cdecl _lua_free(lua_State* state)
{
	free((void*)lua_tointeger(state, 1));
	return 0;
}

static int32_t _cdecl _lua_bzero(lua_State* state)
{
	ZeroMemory((void*)lua_tointeger(state, 1), lua_tointeger(state, 2));
	return 0;
}

static int32_t _cdecl _lua_ptrs_size(lua_State* state)
{
	lua_pushinteger(state, ptrs_size());
	return 1;
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
		MessageBoxA(NULL, ("Error! urlmon.dll! not found! at: " + urlmonpath).c_str(), "Error!", NULL);
	}
}

LuaManager::LuaManager()
{
	m_instance = this;
	m_lua = nullptr;
	m_ptrs = new PointersManager();
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

	if (m_ptrs)
	{
		delete m_ptrs;
		m_ptrs = nullptr;
	}
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
	lua_register(m_lua, "get_argument", _lua_get_argument);
	lua_register(m_lua, "get_argc", _lua_get_argc);
	lua_register(m_lua, "get_setting", _lua_setting);
	lua_register(m_lua, "set_urloutput", _lua_seturloutput);
	lua_register(m_lua, "download", _lua_download);
	lua_register(m_lua, "sleep", _lua_sleep);
	lua_register(m_lua, "get_cpu", _lua_getcpu);
	lua_register(m_lua, "execute", _lua_execute);
	lua_register(m_lua, "release", _lua_release);
	lua_register(m_lua, "malloc", _lua_malloc);
	lua_register(m_lua, "free", _lua_free);
	lua_register(m_lua, "bzero", _lua_bzero);
	lua_register(m_lua, "ptrs_size", _lua_ptrs_size);
	lua_register(m_lua, "set", _lua_set);
	lua_register(m_lua, "get", _lua_get);
	lua_register(m_lua, "realloc", _lua_realloc);

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
