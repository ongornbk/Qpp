#include "stdafx.h"
#include "TextFile.h"

void TextFile::open(const char* path, uint32_t mode)
{
	m_stream.open(path, mode);
}

void TextFile::close()
{
	if (m_stream.is_open())
	{
		m_stream.close();
	}
}

int32_t TextFile::is_open() const
{
	return m_stream.is_open();
}

std::string TextFile::to_string()
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
		MessageBoxA(NULL, "Error! File not opened!", "Error!", MB_OK);
	}
	return ss.str();
}

void TextFile::append_string(std::string str)
{
	if (m_stream.is_open())
	{
		m_stream << str;
	}
	else
	{
		MessageBoxA(NULL, "Error! File not opened!", "Error!", MB_OK);
	}
}