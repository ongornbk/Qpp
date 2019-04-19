#pragma once

struct COLOR
{
	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};
		uint32_t color;

	};

};

struct PointersManager
{
	long(_cdecl* x32)(void* obj);
	void* (_cdecl* x64)(const long obj);
};

extern "C"
{
	struct cpair
	{
		const char* key;
		lua_CFunction value;
	};

	extern __declspec(dllexport) long start(PointersManager* arg);
	extern __declspec(dllexport) long close(const long arg);
	extern __declspec(dllexport) long foo_count(const long arg);
	extern __declspec(dllexport) cpair get_foo(const long index);

}