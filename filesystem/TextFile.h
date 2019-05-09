#pragma once
#include "File.h"

struct TextFile : public File
{
	TextFile() = default;

	~TextFile() = default;

	void open(const char* path, uint32_t mode) override;

	void close() override;

	int32_t is_open() const override;

	std::string to_string();

	void append_string(std::string str);

	std::fstream m_stream;

};