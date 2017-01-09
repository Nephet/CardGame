#include "stdafx.h"
#include "Logger.h"

void Stormancer::Logger::log(const std::string message)
{
	std::cout << message;
	printf("\n");
}

void Stormancer::Logger::log(LogLevel level, const std::string category, const std::string message, const std::string data)
{
	log(ILogger::format(level,category,message,data));
}

void Stormancer::Logger::log(const std::exception & e)
{
	log(LogLevel::Error, "exception", e.what(),"");
}
