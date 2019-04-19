#pragma once

bool release_ptr(const long);

struct PointersManager 
{
	PointersManager();

	long (_cdecl* x32)(void* obj);
	void* (_cdecl* x64)(const long obj);
};