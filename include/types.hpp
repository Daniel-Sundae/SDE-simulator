#pragma once

using TIME = double;
using STATE = double;
enum class SDEType{
    BM = 0,
    GBM,
    Levy,
    OU
};