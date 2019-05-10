#include "stdafx.h"

#include "terminal.wrapper.h"

extern "C"
{

	long start(const long arg)
	{
		HINSTANCE cs_dll{};
		LPFNDLLFUNC1 test{};

		cs_dll = LoadLibrary(L"terminal.dll");
		if (!cs_dll) 
		{ 
			return 1;
		}

		test = (LPFNDLLFUNC1)GetProcAddress(cs_dll,"test");
		if (!test)
		{
			FreeLibrary(cs_dll);
			return 2;
		}
		test();

		return 0;
	}

	long close(const long arg)
	{

		return 0;
	}

	static int32_t test(lua_State* state)
	{

		return 0;
	}

	constexpr long FOO_COUNT = 1;

	const char* sckeys[FOO_COUNT] = {
		"test"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		test
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