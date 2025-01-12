#pragma once
#include <vector>

using Time = double;
using State = double;
using Path = std::vector<double>;
enum class SDEType{
    BM = 0,
    GBM,
    Levy,
    OU
};