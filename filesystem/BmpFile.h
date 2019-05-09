#pragma once
#include "File.h"


struct BmpFile : public File
{
	BmpFile() = default;

	~BmpFile() = default;

	void open(const char* path, uint32_t mode) override;

	void close() override;

	int32_t is_open() const override;

	BITMAPFILEHEADER get_header() const noexcept;

	void read(char* data,size_t size);

	std::ifstream    m_stream;
	BITMAPFILEHEADER m_header;
	static constexpr size_t HEADER_SIZE = sizeof(tagBITMAPFILEHEADER);
	
};
