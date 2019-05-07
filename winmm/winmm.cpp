// winmm.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "winmm.h"

#define SND_SYNC 0x0000
#define SND_ASYNC 0x0001

typedef long(__stdcall * pICFUNC)(LPCSTR, HMODULE, DWORD);

extern "C"
{
#ifdef PlaySound
#undef PlaySound
#endif

	pICFUNC PlaySound = nullptr;
}

struct Winmm;

namespace
{
	static std::unique_ptr<Winmm> m_url = nullptr;
}

struct Winmm
{
	Winmm()
	{
		m_urlmon = LoadLibraryA("winmm.dll");
		PlaySound = pICFUNC(GetProcAddress(m_urlmon, "PlaySoundA"));
	}

	static Winmm* GetInstance()
	{
		if (m_url.get())
		{
			return m_url.get();
		}
		else
		{
			m_url = std::make_unique<Winmm>();
			return m_url.get();
		}
	}

	HMODULE     m_urlmon;
	std::string m_utlpath;

};


extern "C"
{
	long start(const long arg)
	{

		return 0;
	}

	long close(const long arg)
	{
		return 0;
	}

	static int32_t _cdecl _lua_playasync(lua_State* state)
	{
		auto wmm = Winmm::GetInstance();
		std::string str = lua_tostring(state, 1);
		PlaySound(str.c_str(), NULL, SND_ASYNC);
		return 0;
	}

	static int32_t _cdecl _lua_playsync(lua_State* state)
	{
		auto wmm = Winmm::GetInstance();
		std::string str = lua_tostring(state, 1);
		PlaySound(str.c_str(), NULL, SND_SYNC);
		return 0;
	}

	constexpr long FOO_COUNT = 2;

	const char* sckeys[FOO_COUNT] = {
		"PlayAsync",
		"PlaySync"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_playasync,
		_lua_playsync
	};

	long foo_count(const long arg)
	{
		return FOO_COUNT;
	}

	cpair get_foo(const long index)
	{
		cpair cp;
		cp.key = sckeys[index];
		cp.value = scfooes[index];
		return cp;
	}

}