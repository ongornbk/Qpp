#pragma once
#include "IPackage.h"

class UnmanagedPackage : public IPackage
{
public:

	UnmanagedPackage(lua_State* lua, std::string path, std::string name, std::string as);

	UnmanagedPackage(lua_State* lua, std::string path, std::string name);

	~UnmanagedPackage();

	void PushFunction(std::string name, lua_CFunction foo) override;

	void InitializePackage() override;
};

