#pragma once
#include <string>

typedef int32_t(*cfoo) (void *L);

class IPackage
{
public:
	virtual void push(std::string name, cfoo foo) = 0;
	virtual void initialize() = 0;
};