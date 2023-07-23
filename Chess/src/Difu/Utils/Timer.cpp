#include "Timer.h"

#include "Difu/Utils/Logger.h"
#include <raylib.h>

ScopeTimer::ScopeTimer(std::string scopeName)
{
	name = scopeName;
	startTime = GetTime();
}

ScopeTimer::~ScopeTimer()
{
#ifdef _DEBUG
	double end = GetTime();
#endif
	LOG_INFO("{} took: {} sec", name, end - startTime);
}
