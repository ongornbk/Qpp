#pragma once

using namespace System;

extern "C"
{
	struct cpair
	{
		const char* key;
		lua_CFunction value;
	};

	extern __declspec(dllexport) long start(const long arg);
	extern __declspec(dllexport) long close(const long arg);
	extern __declspec(dllexport) long foo_count(const long arg);
	extern __declspec(dllexport) cpair get_foo(const long index);

}

typedef int32_t(CALLBACK* LPFNDLLFUNC1)(void);

namespace terminalwrapper {
	public ref class Class1
	{
		// TODO: Add your methods for this class here.
	};
}
