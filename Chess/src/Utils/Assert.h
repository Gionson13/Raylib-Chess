#pragma once

#include "Logger.h"

#define ENABLE_ASSERT

#if defined(ENABLE_ASSERT) && defined(_DEBUG)
    #define ASSERT(cond, msg)   \
    {                           \
        if (!(cond))            \
        {                       \
            Logger::Error(msg); \
            __debugbreak();     \
        }                       \
    }
#else
    #define ASSERT(cond, msg)
#endif
