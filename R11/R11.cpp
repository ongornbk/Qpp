#include "pch.h"
#include "LuaManager.h"
#include "resource.h"
#include "Settings.h"

#include <iostream>

int CALLBACK WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow
)
{
	std::string exe_path = __argv[0];

	exe_path = exe_path.substr(0,exe_path.find_last_of("/\\")+1);

	InitializeSettings((exe_path + "config.ini").c_str());

	if (__argc < 2)
	{
		MessageBoxA(NULL, "Error! Cannot run without arguments! \n ;)", "Error!", MB_OK);
		return 1;
	}
		LuaManager lm;
		lm.Initialize(__argc, __argv);
	//}
	



	return 0;
}
