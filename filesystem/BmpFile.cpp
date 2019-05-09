#include "stdafx.h"
#include "BmpFile.h"

void BmpFile::open(const char* path, uint32_t mode)
{
	m_stream.open(path, mode);
	m_stream.read((char*)(&m_header), HEADER_SIZE);
}

void BmpFile::close()
{
	if (m_stream.is_open())
	{
		m_stream.close();
	}
}

int32_t BmpFile::is_open() const
{
	return m_stream.is_open();
}

BITMAPFILEHEADER BmpFile::get_header() const noexcept
{
	return m_header;
}

void BmpFile::read(char* data, size_t size)
{
	const size_t sizem = min((size_t)(m_header.bfSize - m_header.bfOffBits), size);

	if (data == nullptr)
	{
		data = (char*)malloc(sizeof(char)*sizem);
	}

	try
	{
		m_stream.read(data, sizem);
	}
	catch (std::exception exe)
	{
		MessageBoxA(NULL, exe.what(), "Error!",MB_OK);
	}
}