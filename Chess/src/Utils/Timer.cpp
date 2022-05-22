#include "Timer.h"

#include "Logger.h"
#include <raylib.h>
#include <string>

ScopeTimer::ScopeTimer(std::string scopeName)
{
    name = scopeName;
    startTime = GetTime();
}

ScopeTimer::~ScopeTimer()
{
    double end = GetTime();
    LOG_INFO(name + " took: " + std::to_string(end - startTime) + " sec");
}
