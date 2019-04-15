#include "pch.h"
#include "Settings.h"

namespace
{
	static std::map<const std::string, String> global_settings;
}

int32 InitializeSettings(const char* filename)
{
	std::ifstream stream(filename);
	if (!stream.good())
	{
		MessageBoxA(NULL, "No config.ini found!", "Error", MB_OK);
		return 1;
	}

	{
		std::string buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

		std::istringstream ss(buffer);
		std::string token;
		while (getline(ss, token, '\n'))
		{
			Pair<std::string, std::string> pair;
			SplitString(pair, token, ' ');
			PushSetting(pair.first, pair.second);
		}

		stream.close();
	}
	return 0;
}

void PushSetting(const std::string key, const std::string value)
{
	global_settings[key] = String(value);
}


String GetSetting(const std::string key)
{
	try
	{
		const String str = global_settings.at(key);
		return str;
	}
	catch (std::exception exe)
	{
		std::string msg = exe.what() + std::string(" : ") + key;
		const int32 retv = MessageBoxA(false, msg.c_str(), "Exception!", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON1);
		switch (retv)
		{
		case 3: //Abort
		{
			PostQuitMessage(-1);
			return String();
			break;
		}
		case 4: //Retry
		{
			return GetSetting(key);
			break;
		}
		case 5:
		{
			return String("Error");
			break;
		}
		default:
		{
			return String("Error");
		}
		}

	}
}