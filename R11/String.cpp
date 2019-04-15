#include "pch.h"
#include "String.h"

String::String()
{

}

String::String(std::string str)
{
	text = str;
}

String::String(std::wstring wstr)
{
	text = std::string(wstr.begin(), wstr.end());
}

String::String(const char *str)
{
	text = str;
}

String::String(const bool boolean)
{
	if (boolean)
		text = "true";
	else text = "false";
}

String::String(const char ch)
{
	text = ch;
}

String::String(const int32 i)
{
	text = std::to_string(i);
}

String::String(const uint32 u)
{
	text = std::to_string(u);
}

String::String(const int64 i)
{
	text = std::to_string(i);
}

String::String(const uint64 u)
{
	text = std::to_string(u);
}

String::String(const double d)
{
	text = std::to_string(d);
}

String::~String()
{
	text.clear();
}

std::string String::get_string() const noexcept
{
	return text;
}

const char* String::c_str() const noexcept
{
	return text.c_str();
}

size_t String::length() const noexcept
{
	return text.length();
}

int32 String::to_int32()
{
	return std::stoi(text);
}

float String::to_float()
{
	return std::stof(text);
}

bool String::to_bool()
{
	if (strcmp("true", text.c_str()))
		return false;
	return true;
}

bool String::is_digit() const
{
	return !text.empty() && std::find_if(text.begin(), text.end(), [](char c) { return !std::isdigit(c); }) == text.end();
}

String& String::operator=(const String& str)
{
	text.clear();
	for (auto c : str.get_string())
	{
		text += c;
	}
	return *this;
}

String& String::operator+=(const char ch)
{
	text.push_back(ch);
	return *this;
}

String& String::operator+=(const char* str)
{
	text += str;
	return *this;
}

bool String::operator==(const String& B) const
{
	if (strcmp(this->c_str(), B.c_str()))
		return false;
	else return true;
}

bool String::operator < (const String& B) const
{
	const size_t length = std::min(text.length(), B.length());
	for (uint64 i = 0u; i < length; i++)
	{
		const auto ch0 = text[i];
		const auto ch1 = B.get_string()[i];
		if (ch0 != ch1) return ch0 < ch1;
	}

	return text.length() < B.length();
}

bool String::operator==(const char* B)
{
	if (strcmp(this->c_str(), B))
		return false;
	else return true;
}

bool String::operator==(const char B)
{
	if (text.length() == 1u && text.at(0) == B)
		return true;
	else return false;
}

bool String::operator!=(const char B)
{
	if (text.length() != 1u || text.at(0) == B)
		return false;
	else return true;
}

bool String::operator!=(const char* B)
{
	return (!(*this == B));
}

void String::clear()
{
	text.clear();
}

void SplitString(std::vector<String> &vec, String &s, char delim)
{
	std::stringstream ss(s.get_string());
	std::string item;
	while (std::getline(ss, item, delim))
		vec.push_back(String(item));
}

void SplitString(std::vector<std::string> &vec, std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		vec.push_back(item);
}

void SplitString(Pair<std::string, std::string> &pair, std::string s, char delim)
{
	pair.first = s.substr(0, s.find_first_of(' '));
	pair.second = s.substr(s.find_last_of(' ') + 1u, s.length());
}

std::string GetFileName(const std::string & s) noexcept
{
	char sep = '/';
#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return (s.substr(i + 1, s.length() - i));
	}
	return ("");
}