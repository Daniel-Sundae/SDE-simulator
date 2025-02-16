#pragma once
using Time = double;
using State = double;
using Path = std::vector<double>;
using StateDot = double; // dX/dt
using Drift = std::function<StateDot(Time, State)>;
using Diffusion = std::function<StateDot(Time, State)>;