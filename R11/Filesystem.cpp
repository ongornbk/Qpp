#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <stack>
#include <exception>
#include <memory>



struct File
{
	File()
	{
	}

	~File()
	{
		
	}

	void open(const char* path,uint32_t mode = 3)
	{
		m_stream.open(path,mode);
	}

	void close()
	{
		if (m_stream.is_open())
		{
			m_stream.close();
		}
	}

	bool is_open()
	{
		return m_stream.is_open();
	}

	std::string to_string()
	{
		std::stringstream ss;
		if (m_stream.is_open())
		{
			while (m_stream.peek() != EOF)
			{
				ss << (char)m_stream.get();
			}
			
		}
		else
		{
			MessageBox(NULL, L"Error! File not opened!", NULL, NULL);
		}
		return ss.str();
	}

	void append_string(std::string str)
	{
		if (m_stream.is_open())
		{
			m_stream << str;
		}
		else
		{
			MessageBox(NULL, L"Error! File not opened!", NULL, NULL);
		}
	}

	std::fstream m_stream;

};

namespace
{
	static lua_State* m_lua = nullptr;
	static std::stack<File*> m_files;
}

static int32_t _cdecl Mkdir(lua_State* state)
{
	lua_pushboolean(state, CreateDirectoryA(lua_tostring(state,1), NULL));
	return 1;
}

static int32_t _cdecl __GetEnv(lua_State* state)
{
#pragma warning(disable : 4996)
	lua_pushstring(state,getenv(lua_tostring(state,1)));
	return 1;
}

static int32_t _cdecl __CopyFile(lua_State* state)
{

#pragma warning(disable : 4996)

	char buf[BUFSIZ];
	size_t size;

	std::string out = lua_tostring(state, 2);
	std::string in = lua_tostring(state, 1);

	FILE* source = fopen(in.c_str(), "rb");
	FILE* dest = fopen(out.c_str(), "wb");

	// clean and more secure
	// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

	while (size = fread(buf, 1, BUFSIZ, source)) {
		fwrite(buf, 1, size, dest);
	}

	fclose(source);
	fclose(dest);

	return 0;
}

static int32_t _cdecl __RemoveFile(lua_State* state)
{
#pragma warning(disable : 4996)
	std::string file = lua_tostring(state, 1);
	remove(file.c_str());
	return 0;
}

static int32_t _cdecl __OpenFileAppend(lua_State* state)
{

	std::string path = lua_tostring(state, 1);
	File* file = new File();
	file->open(path.c_str(),std::ios::out | std::ios::in | std::ios::app);
	if (file->is_open())
	{
		m_files.push(file);
	}
	else
	{
		MessageBox(NULL, L"Error! File not opened!", NULL, NULL);
	}
	return 0;
}

static int32_t _cdecl __OpenFileTrunc(lua_State* state)
{
	std::string path = lua_tostring(state, 1);
	File* file = new File();
	file->open(path.c_str(), std::ios::out | std::ios::in | std::ios::trunc);
	if (file->is_open())
	{
		m_files.push(file);
	}
	else
	{
		MessageBox(NULL, L"Error! File not opened!", NULL, NULL);
	}
	return 0;
}

static int32_t _cdecl __CloseFile(lua_State* state)
{
	if (m_files.empty())
	{
		MessageBox(NULL, L"Error! No file to close!",NULL,NULL);
	}
	else
	{
		File* file = m_files.top();
		if (file)
		{
			file->close();
			delete file;
			file = nullptr;
		}
		m_files.pop();

	}
	return 0;
}

static int32_t _cdecl __LoadFileToString(lua_State* state)
{
	std::string str;
	if (m_files.empty())
	{
		MessageBox(NULL, L"Error! No files to load!", NULL, NULL);
	}
	else
	{
		str = m_files.top()->to_string();
	}
	lua_pushstring(m_lua, str.c_str());
	return 1;
}

static int32_t _cdecl __AppendStringToFile(lua_State* state)
{
	std::string str = lua_tostring(m_lua, 1);
	if (m_files.empty())
	{
		MessageBox(NULL, L"Error! No files to load!", NULL, NULL);
	}
	else
	{
		m_files.top()->append_string(str);
	}
	return 0;
}

void _stdcall FilesystemPackageInitializer()
{

	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "Mkdir", Mkdir);
	lua_register(m_lua, "GetEnv", __GetEnv);
	lua_register(m_lua, "CopyFile", __CopyFile);
	lua_register(m_lua, "RemoveFile", __RemoveFile);
	lua_register(m_lua, "OpenFileAppend", __OpenFileAppend);
	lua_register(m_lua, "OpenFileTrunc", __OpenFileTrunc);
	lua_register(m_lua, "CloseFile", __CloseFile);
	lua_register(m_lua, "LoadFileToString", __LoadFileToString);
	lua_register(m_lua, "AppendStringToFile", __AppendStringToFile);
}