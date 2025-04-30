## Stochastic Differential Equation Simulator
A GUI program for simulating and visualising Stochastic Differential Equations (SDEs) using C++20.  
Based on the Qt framework.  
Supports Brownian Motion, Geometric Brownian Motion and Ornstein-Uhlenbeck processes out of the box.  
Supports Euler-Maruyama, Runge-Kutta and Milstein solvers.  
Supports multithreaded path sampling.  
<img src="./images/example.png" alt="Example picture" width="80%"/>

## Build Prerequisites
- CMake 3.22+
- Qt6 (Qt5 should be fine) with MinGW. Can be installed for free here: https://www.qt.io/download-qt-installer-oss
- Ninja build system

## Build Instructions (Windows)

1: Clone the project into your repository
```bash
git clone https://github.com/Daniel-Sundae/SDE-simulator.git
```
2: Set environment variables QTDIR_MINGW and MINGW_PATH to your local QT directories
```bash
# Example in windows command prompt where Qt is downloaded under C:
set QTDIR_MINGW=C:\Qt\6.8.3\mingw_64
set MINGW_PATH=C:\Qt\Tools\mingw1310_64
```

3: Configure the project
```bash
# If you want no tests
cmake --preset=release-mingw -DBUILD_TESTING=OFF

# If you want tests
cmake --preset=release-mingw
```

4: Build the project
```bash
cmake --build build/Release-MINGW
```

Find the executable at build/Release-MINGW/StochasticSim.exe
and the test executable at build/Release-MINGW/Tests/StochasticSimTests.exe
