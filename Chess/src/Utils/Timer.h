#pragma once

#include <iostream>

class ScopeTimer
{
public:
    ScopeTimer() = delete;
    ScopeTimer(std::string scopeName);

    ~ScopeTimer();

private:
    double startTime;
    std::string name;
};