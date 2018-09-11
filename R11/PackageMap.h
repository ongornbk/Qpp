#pragma once
#include "Package.h"
#include <unordered_map>

class PackageMap
{
public:
	static std::unordered_map<std::string, Package*> m_packages;
};

