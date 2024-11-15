#pragma once

#include <vector>
#include "Process.hpp"

class Wienerprocess: public Process{
public:
    Wienerprocess();
    std::vector<Process::Point> Sample() const final; 
};