## Build Prerequisites
- CMake 3.22+
- Qt6 (Qt5 should be fine) with MinGW. Can be installed for free here: https://www.qt.io/download-qt-installer-oss
- Ninja build system

## Build Instructions

1: Clone the project into your repository
```bash
git clone https://github.com/Daniel-Sundae/Stochastic-Differential-Equations.git
```
2: Set environment variables QTDIR_MINGW and MINGW_PATH to your local directories
```bash
# Example in windows command prompt where Qt is downloaded under C:
set QTDIR_MINGW=C:\Qt\6.8.3\mingw_64
set MINGW_PATH=C:\Qt\Tools\mingw1310_64
```

3: Configure the project
```bash
# Set testing flag on if you want to build tests
cmake --preset=release-mingw -DBUILD_TESTING=OFF
```

4: Build the project
```bash
cmake --build build/Release-MINGW
```

5: Run the executable
```bash
build/Release-MINGW/StochasticSim.exe
```