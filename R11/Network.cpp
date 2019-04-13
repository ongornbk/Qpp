#pragma once
#include "pch.h"
#include "Network.h"
#include "LuaManager.h"
//#include "Enet.h"


namespace
{
	static lua_State* m_lua = nullptr;
//	static ENetAddress address;
//	static ENetHost * server;
//	static ENetHost * client;
//	static ENetEvent event;
//	static ENetPeer *peer;

}


static int32_t _cdecl HostCreate(lua_State* state)
{
	bool result = false;

	//address.host = ENET_HOST_ANY;
	//
	//address.port = lua_tointeger(state,1);
	//server = enet_host_create(&address /* the address to bind the server host to */,
	//	lua_tointeger(state,2)      /* allow up to 32 clients and/or outgoing connections */,
	//	2      /* allow up to 2 channels to be used, 0 and 1 */,
	//	0      /* assume any amount of incoming bandwidth */,
	//	0      /* assume any amount of outgoing bandwidth */);
	//
	//result = (bool)(server);
	//
	//lua_pushboolean(state, result);
	return 1;
}





static int32_t _cdecl HostDestroy(lua_State* state)
{
//	enet_host_destroy(server);
	return 0;
}

static int32_t _cdecl ClientCreate(lua_State* state)
{
	bool result = false;


//	client = enet_host_create(NULL /* the address to bind the server host to */,
	//	lua_tointeger(state, 1)      /* allow up to 32 clients and/or outgoing connections */,
	//	2      /* allow up to 2 channels to be used, 0 and 1 */,
	//	0      /* assume any amount of incoming bandwidth */,
	//	0      /* assume any amount of outgoing bandwidth */);

	//result = (bool)(client);

//	lua_pushboolean(state, result);
	return 1;
}


static int32_t _cdecl ClientDestroy(lua_State* state)
{
//	enet_host_destroy(client);
	return 0;
}

void CALL_CONV NetworkPackageInitializer()
{

	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "ClientCreate", ClientCreate);
	lua_register(m_lua, "HostDestroy", HostDestroy);
	lua_register(m_lua, "HostCreate", HostCreate);
	lua_register(m_lua, "ClientDestroy", ClientDestroy);

}


