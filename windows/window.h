#pragma once

extern lua_CFunction EventTimer;
extern lua_CFunction EventDestroy;
extern lua_CFunction EventPaint;
extern lua_CFunction EventHover;

extern LRESULT __stdcall WindowProcedure(HWND window, uint32_t msg, WPARAM wp, LPARAM lp);

void set_lua(lua_State* state);

class Window
{
public:

	Window();

	~Window();

	HWND window;
};