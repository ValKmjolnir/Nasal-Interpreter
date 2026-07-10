# Build Nasal-Interpreter on Windows

## MSVC / Visual Studio

Need CMake and Visual Studio. Remember to add MSBuild.exe to Path.

Valid on powershell:

```sh
mkdir cmake-windows-msvc
cd cmake-windows-msvc
cmake ..
cmake --build . --config Release -j 6
```

## MingW-W64

Need CMake and MingW-W64. Remember to add MingW-W64 bin to Path.

Valid on powershell:

```sh
mkdir cmake-windows-mingw
cd cmake-windows-mingw
cmake .. -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
mingw32-make.exe -j6
```
