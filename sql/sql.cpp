// sql.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "sql.h"
#include "sqlite3.h"

namespace
{
	std::queue<std::string> m_returnmsg;
}

extern "C"
{

	static int callback(void *NotUsed, int argc, char **argv, char **azColName)
	{
		int i;
		for (i = 0; i < argc; i++)
		{
			m_returnmsg.push(std::string( azColName[i] + std::string(" = ") + (argv[i] ? argv[i] : "NULL") + "\n"));
		}
		return 0;
	}

		long start(const long arg)
		{
			return 0;
		}

		long close(const long arg)
		{
			while (m_returnmsg.size())
				m_returnmsg.pop();

			return 0;
		}

		int32_t _cdecl _lua_open(lua_State* state)
		{
			sqlite3* db{};
			const long result = sqlite3_open(lua_tostring(state, 1),&db);
			lua_pushinteger(state, (lua_Integer)db);
			return 1;
		}

		int32_t _cdecl _lua_close(lua_State* state)
		{
			sqlite3_close((sqlite3*)lua_tointeger(state, 1));
			return 0;
		}

		int32_t _cdecl _lua_exec(lua_State* state)
		{
			char *zErrMsg = 0;
			const long result = sqlite3_exec((sqlite3*)lua_tointeger(state, 1), lua_tostring(state, 2), callback, 0, &zErrMsg);
			if (result != SQLITE_OK)
			{
				sqlite3_free(zErrMsg);
				lua_pushinteger(state, -1);

			}
			else
			{
				lua_pushinteger(state, m_returnmsg.size());
			}
			return 1;
		}

		int32_t _cdecl _lua_extract(lua_State* state)
		{
			lua_pushstring(state, m_returnmsg.front().c_str());
			m_returnmsg.pop();
			return 1;
		}

		constexpr long FOO_COUNT = 4;

		const char* sckeys[FOO_COUNT] = {
			"Close",
			"Exec",
			"Extract",
			"Open"
		};
		const lua_CFunction scfooes[FOO_COUNT] = {
			_lua_close,
			_lua_exec,
			_lua_extract,
			_lua_open
		};

		long foo_count(const long arg)
		{
			return FOO_COUNT;
		}

		cpair get_foo(const long index)
		{
			cpair cp;
			cp.key = sckeys[index];
			cp.value = scfooes[index];
			return cp;
		}
}

