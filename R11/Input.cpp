#include "pch.h"
#include "Input.h"
#include <memory>
#include <mutex>
#include "LuaManager.h"
#include "DirectInput.h"
#include "WindowC.h"

#define GWL_HINSTANCE (-6)

struct DInput
{
	DInput()
	{
		RECT rect;
		GetWindowRect(m_pickedWindow, &rect);
		m_instance.Initialize((HINSTANCE)GetWindowLong(m_pickedWindow, GWL_HINSTANCE), m_pickedWindow, rect.right-rect.left, rect.bottom-rect.top);
	}
	Input m_instance;
};

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<DInput> m_input = nullptr;
}

static int32_t _cdecl UpdateInput(lua_State* state)
{
	m_input->m_instance.Update();
	return 0;
}

static int32_t __GetMousePressed(lua_State* state)
{
	lua_pushboolean(state, m_input->m_instance.GetMousePressed((int32_t)lua_tointeger(state, 1)));
	return 1;
}

static int32_t __GetMouseState(lua_State* state)
{
	if (m_input->m_instance.GetMouseState((uint32_t)lua_tointeger(state, 1)))
	{
		lua_pushboolean(state, true);
	}
	else
	{
		lua_pushboolean(state, false);
	}
	return 1;
}
