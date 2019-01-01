#include "pch.h"
#include <vector>
#include "Collector.h"

std::vector<void*> thrash;

void _cdecl push_ptr(void * ptr)
{
	thrash.push_back(ptr);
}

void _cdecl free_collector()
{
	for (auto && ptr : thrash)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
}
