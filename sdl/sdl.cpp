// sdl.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "sdl.h"

#pragma comment(lib,"SDL2.lib")

union u32u
{
	struct
	{
		unsigned char x;
		unsigned char y;
		unsigned char z;
		unsigned char w;
	};

	unsigned long vl;
};

namespace
{
	static SDL_Window*   m_window{};
	static SDL_Renderer* m_renderer{};
	static u32u           m_bgcolor{};
}



extern "C"
{


	long start(const long arg)
	{
		return 0;
	}

	long close(const long arg)
	{

		return 0;
	}

	static int32_t _cdecl lua_initializewindow(lua_State* state)
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		m_window = SDL_CreateWindow(
			lua_tostring(state,1),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			lua_tointeger(state,2),
			lua_tointeger(state,3),
			lua_tointeger(state,4));

		if (!m_window)
		{
			lua_pushinteger(state, 1);
			return 1;
		}
		m_renderer = SDL_CreateRenderer(
			m_window,
			0,
			SDL_RENDERER_ACCELERATED);
		if (m_renderer)
		{
			SDL_SetRenderDrawColor(m_renderer, m_bgcolor.x, m_bgcolor.y, m_bgcolor.z, m_bgcolor.w);
			SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		}
		else
		{
			lua_pushinteger(state, 2);
			return 1;
		}

		lua_pushinteger(state, 0);
		return 1;

	}

	static int32_t _cdecl lua_releasewindow(lua_State* state)
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return 0;
	}

	static int32_t _cdecl lua_runwindow(lua_State* state)
	{

		int32_t running = 1;

		while (running)
		{
			SDL_Event event{};
			SDL_PollEvent(&event);
			switch (event.type)
			{
			case SDL_QUIT:
			{
				running = 0;
				continue;
			}
			}

			SDL_RenderClear(m_renderer);

			SDL_RenderPresent(m_renderer);
		}

		return 0;
	}


	constexpr long FOO_COUNT = 3;

	const char* sckeys[FOO_COUNT] = {
		"CreateWindow",
		"ReleaseWindow",
		"RunWindow"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		lua_initializewindow,
		lua_releasewindow,
		lua_runwindow
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

