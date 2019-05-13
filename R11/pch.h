#pragma once
#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <sal.h>
#include <mmsystem.h>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <type_traits>
#include <cctype>
#include <vector>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <stack>
#include <list>

#include "luaH.h"

#pragma region
typedef signed char            int8;
typedef const signed char      cint8;
typedef signed short           int16;
typedef const signed short     cint16;
typedef signed long            int32;
typedef const signed long      cint32;
typedef signed long long       int64;
typedef const signed long long cint64;

typedef unsigned char          uint8;
typedef const unsigned char    cuint8;
typedef unsigned short         uint16;
typedef const unsigned short   cuint16;
typedef unsigned long          uint32;
typedef const unsigned long    cuint32;
typedef unsigned long long     uint64;
typedef const unsigned long    cuint64;

#pragma endregion

#pragma region
template <class _Out_ ReturnType, class _In_ Type>
inline ReturnType& memory_cast(const Type& obj)
{
	return *(ReturnType*)(obj);
}

template <class _Out_ ReturnType, class _In_ Type>
inline ReturnType* mmalloc(const Type nitems)
{
	return (ReturnType*)malloc((nitems) * sizeof(ReturnType));
}

template <class _Out_ ReturnType, class _In_ Type>
inline ReturnType* mcalloc(const Type nitems)
{
	return (ReturnType*)calloc(nitems, sizeof(ReturnType));
}

template <class ReturnType, class A, class _In_ Type>
inline ReturnType* mrealloc(A& ptr, const Type nitems)
{
	return (ReturnType*)realloc(ptr, (nitems) * sizeof(ReturnType));
}

template <class A>
inline void mfree(A& ptr)
{
	return free(ptr);
}
#pragma endregion

extern "C"
{
	struct cpair
	{
		constexpr cpair(const char* _key,lua_CFunction _value) : key(_key), value(_value) {}

		const char* key;
		lua_CFunction value;
	};
}



