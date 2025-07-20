#pragma once
#include <functional>

class FunctionWrapper {
protected:
    
    // mu or sigma
    const double parameter;

    // a(t, Xt) or b(t, Xt) in dXt = adt + bdB
    const std::function<double(Time, State)> f;

    // da/dX or db/dX
    const std::function<double(Time, State)> fPrime;

public:
    FunctionWrapper(double _parameter, std::function<double(Time, State)> _f, std::function<double(Time, State)> _fPrime = [](Time, State){return 0;})
        : parameter(_parameter)
        , f(_f)
        , fPrime(_fPrime)
    {
    }
    double operator()(Time t, State s) const { return f(t, s); }
    double prime(Time t, State s) const{ return fPrime(t, s); }
};

class Drift : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    double mu() const{ return parameter; }
};

class Diffusion : public FunctionWrapper {
    using FunctionWrapper::FunctionWrapper;
public:
    double sigma() const{ return parameter; }
};