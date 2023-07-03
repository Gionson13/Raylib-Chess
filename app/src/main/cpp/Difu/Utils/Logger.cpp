#include "Logger.h"
#include <string>

namespace Logger
{
	static void(*coutFunc)(std::string) = nullptr;

	void Log(std::string message)
	{
		if (coutFunc)
			coutFunc(message);
	}

	void Bind(void (*func)(std::string))
	{
		coutFunc = func;
	}
}
