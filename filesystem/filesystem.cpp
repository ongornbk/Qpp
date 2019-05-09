// filesystem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "filesystem.h"
#include "TextFile.h"
#include "BmpFile.h"

namespace
{
	static std::stack<File*> s_files;

}

extern "C"
{
	long start(const long arg)
	{

		return 0;
	}

	long close(const long arg)
	{
		while (s_files.size())
		{
			File* top = s_files.top();
			if (top)
			{
				top->close();
				delete top;
				top = nullptr;
			}
			s_files.pop();
		}

		return 0;
	}

	static int32_t _cdecl _lua_mkdir(lua_State* state)
	{
		lua_pushboolean(state, CreateDirectoryA(lua_tostring(state, 1), NULL));
		return 1;
	}

	static int32_t _cdecl _lua_copyfile(lua_State* state)
	{

#pragma warning(disable : 4996)

		char buf[BUFSIZ];
		size_t size;

		std::string out = lua_tostring(state, 2);
		std::string in = lua_tostring(state, 1);

		FILE* source = fopen(in.c_str(), "rb");
		if (!source)
		{
			MessageBoxA(NULL, ("Cannot open: " + in).c_str(), "Error!", MB_OK);
			return 0;
		}

		FILE* dest = fopen(out.c_str(), "wb");
		if (!dest)
		{
			MessageBoxA(NULL, ("Cannot open: " + out).c_str(), "Error!", MB_OK);
			return 0;
		}

		// clean and more secure
		// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

		while (size = fread(buf, 1, BUFSIZ, source)) {
			fwrite(buf, 1, size, dest);
		}

		fclose(source);
		fclose(dest);

		return 0;
	}

	static int32_t _cdecl _lua_removefile(lua_State* state)
{
#pragma warning(disable : 4996)
	remove(lua_tostring(state, 1));
	return 0;
}

	static int32_t _cdecl _lua_openappend(lua_State* state)
	{

		File* file = new TextFile();
		file->open(lua_tostring(state, 1), std::ios::out | std::ios::in | std::ios::app);
		if (file->is_open())
		{
			s_files.push(file);
		}
		else
		{
			MessageBoxA(NULL, "Error! File not opened!", "Error!", MB_OK);
		}
		return 0;
	}

	static int32_t _cdecl _lua_opentrunc(lua_State* state)
	{
		File* file = new TextFile();
		file->open(lua_tostring(state, 1), std::ios::out | std::ios::in | std::ios::trunc);
		if (file->is_open())
		{
			s_files.push(file);
		}
		else
		{
			MessageBoxA(NULL, "Error! File not opened!", "Error!", MB_OK);
		}
		return 0;
	}

	static int32_t _cdecl _lua_openbmp(lua_State* state)
	{
		File* file = new BmpFile();
		file->open(lua_tostring(state, 1),0);
		if (file->is_open())
		{
			s_files.push(file);
		}
		else
		{
			MessageBoxA(NULL, "Error! File not opened!", "Error!", MB_OK);
		}
		return 0;
	}

	static int32_t _cdecl _lua_close(lua_State* state)
	{
		if (s_files.empty())
		{
			MessageBoxA(NULL, "Error! No file to close!", "Error!", MB_OK);
		}
		else
		{
			File* top = s_files.top();
			if (top)
			{
				top->close();
				delete top;
				top = nullptr;
			}
			s_files.pop();
		}
		return 0;
	}

	static int32_t _cdecl _lua_tostring(lua_State* state)
	{
		std::string str;
		if (s_files.empty())
		{
			MessageBoxA(NULL, "Error! No files to load!", "Error!", MB_OK);
		}
		else
		{
			TextFile* textFile = dynamic_cast<TextFile*>(s_files.top());
			if(textFile)
			str = textFile->to_string();
		}
		lua_pushstring(state, str.c_str());
		return 1;
	}

	static int32_t _cdecl _lua_appendstring(lua_State* state)
	{
		if (s_files.empty())
		{
			MessageBoxA(NULL, "Error! No files loaded!", "Error!", MB_OK);
		}
		else
		{
			TextFile* textFile = dynamic_cast<TextFile*>(s_files.top());
			if (textFile)
			textFile->append_string(lua_tostring(state, 1));
		}
		return 0;
	}

	static int32_t _cdecl _lua_setattributes(lua_State* state)
	{
		std::string str = lua_tostring(state, 1);
		const DWORD attribute = (DWORD)lua_tointeger(state, 2);
		lua_pushboolean(state,(int)SetFileAttributesA(str.c_str(), attribute));
		return 0;
	}

	static int32_t _cdecl _lua_bmpread(lua_State* state)
	{
		BmpFile* bmp = dynamic_cast<BmpFile*>(s_files.top());
		if (bmp)
		{ 
			char* data = (char*)malloc((size_t)lua_tointeger(state, 1));
			bmp->read(data, (size_t)lua_tointeger(state, 1));
			lua_pushinteger(state, (lua_Integer)data);
			return 1;
		}
		lua_pushinteger(state, 0);
		return 1;
	}

	static int32_t _cdecl _lua_getbmpheader(lua_State* state)
	{
		
			if (s_files.empty())
			{
				MessageBoxA(NULL, "Error! No files loaded!", "Error!", MB_OK);
			}
			else
			{
				BmpFile* bmp = dynamic_cast<BmpFile*>(s_files.top());
				if (bmp)
				{
					BITMAPFILEHEADER temp = bmp->get_header();
					lua_pushinteger(state, (lua_Integer)temp.bfType);
					lua_pushinteger(state, (lua_Integer)temp.bfSize);
					lua_pushinteger(state, (lua_Integer)temp.bfReserved1);
					lua_pushinteger(state, (lua_Integer)temp.bfReserved2);
					lua_pushinteger(state, (lua_Integer)temp.bfOffBits);
					return 5;
				}
				
				
			}
			lua_pushinteger(state,0);
			lua_pushinteger(state,0);
			lua_pushinteger(state,0);
			lua_pushinteger(state,0);
			lua_pushinteger(state,0);
		return 5;
	}

	constexpr long FOO_COUNT = 12;

	const char* sckeys[FOO_COUNT] = {
	"AppendString",
	"BmpRead",
	"Close",
	"CopyFile",
	"GetBmpHeader",
	"Mkdir",
	"OpenAppend",
	"OpenBmp",
	"OpenTrunc",
	"Remove",
	"SetAttributes",
	"ToString"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
	_lua_appendstring,
	_lua_bmpread,
	_lua_close,
	_lua_copyfile,
	_lua_getbmpheader,
	_lua_mkdir,
	_lua_openappend,
	_lua_openbmp,
	_lua_opentrunc,
	_lua_removefile,
	_lua_setattributes,
	_lua_tostring
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

