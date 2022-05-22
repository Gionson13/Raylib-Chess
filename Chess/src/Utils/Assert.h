#pragma once

#include "Logger.h"

#define ENABLE_ASSERT

#ifdef ENABLE_ASSERT
    #define ASSERT(cond, msg)   \
    {                           \
        if (!(cond))            \
        {                       \
            Logger::Error(msg); \
            __debugbreak();     \
        }                       \
    }
#elif
    #define ASSERT(cond, msg)
#endif