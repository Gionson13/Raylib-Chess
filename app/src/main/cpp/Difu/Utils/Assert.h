#pragma once

#include "../Utils/Logger.h"

#define ENABLE_ASSERT

#if defined(ENABLE_ASSERT) && defined(_DEBUG)
	#define ASSERT(cond, msg)   \
	{                           \
		if (!(cond))            \
		{                       \
			Logger::Error(msg); \
			exit(1);     \
		}                       \
	}
#else
	#define ASSERT(cond, msg)
#endif
