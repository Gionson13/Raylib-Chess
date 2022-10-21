#include "Timer.h"

#include "Logger.h"
#include <raylib.h>

ScopeTimer::ScopeTimer(std::string scopeName)
{
	name = scopeName;
	startTime = GetTime();
}

ScopeTimer::~ScopeTimer()
{
	double end = GetTime();
	LOG_INFO("{} took: {} sec", name, end - startTime);
}
