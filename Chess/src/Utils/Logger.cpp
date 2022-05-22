#include "Logger.h"

#define WHITE "\e[0;37m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define RED "\e[0;31m"

void Logger::Debug(std::string message)
{
    std::cout << "[DEBUG]: " << message << std::endl;
}

void Logger::Info(std::string message)
{
    std::cout << GREEN << "[INFO]: " << message << WHITE << std::endl;
}

void Logger::Warn(std::string message)
{
    std::cout << YELLOW << "[WARNING]: " << message << WHITE << std::endl;
}

void Logger::Error(std::string message)
{
    std::cout << RED "[ERROR]: " << message << WHITE << std::endl;
}
