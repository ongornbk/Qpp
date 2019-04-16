#include "pch.h"
#include "PointersManager.h"
#include "Object.h"

struct ptrc
{
	long  index;
	void* ptr;
};

namespace
{
	static long m_index = 0;
	static std::map<void*,ptrc> m_objects;
}

template <class _Out_ x32_type, class _In_ x64_type>
x32_type& x32(const x64_type& obj)
{
	void* in = (void*)(obj)


		size_t = m_objects.find(in);
	if (size_t != m_objects.npos)
	{
		return (x32_type)m_objects[in].index;
	}
	else
	{
		m_index++;
		m_objects[in] = m_index
		return(x32_type)m_objects[in].index;
	}
}

template <class _Out_ x32_type, class _In_ x64_type>
x64_type& x32(const x32_type& obj)
{
	void* in = (void*)(obj)


		size_t = m_objects.find(in);
	if (size_t != m_objects.npos)
	{
		return (x32_type)m_objects[in].index;
	}
	else
	{
		m_index++;
		m_objects[in] = m_index
			return(x32_type)m_objects[in].index;
	}
}
