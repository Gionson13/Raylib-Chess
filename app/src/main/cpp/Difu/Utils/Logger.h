#pragma once

#include <iostream>
//#include <fmt/core.h>

#ifdef _DEBUG
	#define LOG_INFO(msg, ...)  Logger::Info(msg, ##__VA_ARGS__)
	#define LOG_WARN(msg, ...)  Logger::Warn(msg, ##__VA_ARGS__)
	#define LOG_ERROR(msg, ...) Logger::Error(msg, ##__VA_ARGS__)
	#define LOG_DEBUG(msg, ...) Logger::Debug(msg, ##__VA_ARGS__)
#else
	#define LOG_INFO(msg, ...)
	#define LOG_WARN(msg, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_DEBUG(msg, ...)
#endif


#define CONSOLE_WHITE "\e[0;37m"
#define CONSOLE_GREEN "\e[0;32m"
#define CONSOLE_YELLOW "\e[0;33m"
#define CONSOLE_RED "\e[0;31m"


namespace Logger
{
	void Log(std::string message);
	void Bind(void(*func)(std::string));

	template <typename... T>
	void Debug(std::string message, T&&... args)
	{
		std::string finalMessage = "[DEBUG]: " + message;
		std::cout << finalMessage << std::endl;
		Log(finalMessage);
	}
    
	template <typename... T>
	void Info(std::string message, T&&... args)
	{
		std::string finalMessage = "[INFO]: " + message;
		std::cout << CONSOLE_GREEN << finalMessage << CONSOLE_WHITE << std::endl;
		Log(finalMessage);
	}
    
	template <typename... T>
	void Warn(std::string message, T&&... args)
	{
		std::string finalMessage = "[WARNING]: " + message;
		std::cout << CONSOLE_YELLOW << finalMessage << CONSOLE_WHITE << std::endl;
		Log(finalMessage);
	}

	template <typename... T>
	void Error(std::string message, T&&... args)
	{
		std::string finalMessage = "[ERROR]: " + message;
		std::cout << CONSOLE_RED << finalMessage << CONSOLE_WHITE << std::endl;
		Log(finalMessage);
	}
} // namespace Logger
