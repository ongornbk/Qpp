#pragma once
#include "Pair.h"

class String
{
	std::string text;

public:
	String();
	~String();
	explicit String(std::string str);
	explicit String(std::wstring wstr);
	explicit String(const char*);
	explicit String(const bool);
	explicit String(const char);
	explicit String(const int32);
	explicit String(const int64);
	explicit String(const uint32);
	explicit String(const uint64);
	explicit String(const double);

	std::string get_string() const noexcept;
	const char* c_str() const noexcept;

	size_t length() const noexcept;

	float to_float();

	int32 to_int32();

	bool to_bool();
	bool is_digit() const;

	String& operator = (const String& str);

	String& operator += (const char ch);
	String& operator += (const char* str);

	bool operator ==(const String& B) const;
	bool operator ==(const char* B);
	bool operator ==(const char B);
	bool operator !=(const char B);
	bool operator !=(const char* B);

	bool operator <(const String& B) const;

	void clear();
};

void SplitString(std::vector<String> &vec, String &s, char delim);
void SplitString(std::vector<std::string> &vec, std::string &s, char delim);
void SplitString(Pair<std::string, std::string> &pair, std::string s, char delim);
std::string GetFileName(const std::string & s) noexcept;
