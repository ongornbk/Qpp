#pragma once

struct File
{
	virtual void open(const char* path, const uint32_t mode) = 0;
	virtual void close() = 0;
	virtual int32_t is_open() const = 0;
};
