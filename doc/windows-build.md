# Build Nasal-Interpreter on Windows

## MSVC / Visual Studio

Need CMake and Visual Studio 2022. Remember to add MSBuild.exe to Path.

Valid on powershell:

```sh
mkdir cmake-windows-msvc
cd cmake-windows-msvc
cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022"
MSbuild.exe nasal.sln /p:Configuration=Release /p:Platform=x64
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
