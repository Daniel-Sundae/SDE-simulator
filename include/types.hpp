#pragma once
#include <vector>

using Time = double;
using State = double;
using Path = std::vector<double>;
enum class ProcessType{
    NONE = 0,
    BM,
    GBM,
    Levy,
    OU
};
struct ProcessDefinition {
    double mu = 0;
    double sigma = 0;
    double startValue = 0;
};