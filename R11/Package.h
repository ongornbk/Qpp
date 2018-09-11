#pragma once
#include <string>
struct Package
{
public:
	Package();
	~Package();

	void Import();

	void(*m_initializer)(void);
	std::string m_name;
};

