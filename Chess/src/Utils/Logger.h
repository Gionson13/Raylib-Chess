#pragma once

#include <iostream>

#ifdef _DEBUG
    #define LOG_INFO(msg) Logger::Info(msg)
    #define LOG_WARN(msg) Logger::Warn(msg)
    #define LOG_ERROR(msg) Logger::Error(msg)
    #define LOG_DEBUG(msg) Logger::Debug(msg)
#else
    #define LOG_INFO(msg)
    #define LOG_WARN(msg)
    #define LOG_ERROR(msg)
    #define LOG_DEBUG(msg)
#endif

namespace Logger
{
    // TODO: Use string formatter like printf: "%s", "string"
    void Debug(std::string message);
    void Info(std::string message);
    void Warn(std::string message);
    void Error(std::string message);
} // namespace Logger
