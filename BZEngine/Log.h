#pragma once

#include "Headers.h"

#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>

#define LOG_DEBUG

class Log
{
public:
	static void Debug(const std::string& message)
	{
#ifdef LOG_DEBUG
		Print("DEBUG", message);
#endif
	}

	static void Print(const std::string& level, const std::string& message)
	{
		std::cout << currentTimeIso8601_() << " [" << level << "]: " << message << std::endl;
	}

private:
	static std::string currentTimeIso8601_()
	{
		time_t now;
		time(&now);
		char buf[sizeof("0000-00-00T00:00:00Z")];
		tm tm{};
		gmtime_s(&tm, &now);
		strftime(buf, sizeof(buf), "%FT%TZ", &tm);
		return std::string(buf);
	}
};

