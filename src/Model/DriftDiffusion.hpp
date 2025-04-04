#pragma once
#include <functional>

class FunctionWrapper {
protected:
    const double parameter;
    const std::function<StateDot(Time, State)> f;
public:
    FunctionWrapper()
        : parameter(0)
        , f([](Time, State) -> StateDot { return 0; })
    {
    }
    FunctionWrapper(double _parameter, std::function<StateDot(Time, State)> _f)
        : parameter(_parameter)
        , f(_f)
    {
    }
    auto operator()(Time t, State s) const -> StateDot { return f(t, s); }
};

class Drift : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Mu() const -> double { return parameter; }
};

class Diffusion : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    auto Sigma() const -> double { return parameter; }
};