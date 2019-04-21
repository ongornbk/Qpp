#include "pch.h"
#include "PointersManager.h"



namespace
{
	static uint32 used_ptrs = 0;
	std::stack<long> ready_to_use_ptrs;
	static std::map<void*,long> m_objects;
	static std::map<long, void*> m_ptrs;
}

bool release_ptr(const long obj)
{
	std::map<long, void*>::iterator it = m_ptrs.find(obj);
	if (it != m_ptrs.end())
	{
		m_ptrs.erase(it);
		ready_to_use_ptrs.push(obj);
		return true;
	}
	else
	{
		return false;
	}
}

long ptrs_size()
{
	return used_ptrs;
}

long _x32(void* obj)
{
	std::map<void*, long>::iterator it = m_objects.find(obj);
	if (it != m_objects.end())
	{
		return it->second;
	}
	else
	{
		if (ready_to_use_ptrs.empty())
		{
			const long key = used_ptrs;
			m_objects[obj] = key;
			m_ptrs[key] = obj;
			used_ptrs++;
			return key;
		}
		else
		{
			const long key = ready_to_use_ptrs.top();
			ready_to_use_ptrs.pop();
			m_objects[obj] = key;
			m_ptrs[key] = obj;
			return key;
		}
	}
}

void*& _x64(const long obj)
{
	std::map<long, void*>::iterator it = m_ptrs.find(obj);
	if (it == m_ptrs.end())
	{
		std::string msg = "Error! " + std::to_string(obj) + " is not referenced!";
		MessageBoxA(NULL, msg.c_str(), "Error!", NULL);
	}
		return it->second;
}

PointersManager::PointersManager()
{
	this->x32 = _x32;
	this->x64 = _x64;
}