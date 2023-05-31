:: Generate CMake release build

set PATH=%PATH%;C:\Qt\Tools\CMake_64\bin

:: Create build directory
cd..
mkdir cmake-build-release
cd cmake-build-release

:: Start build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

:: Commands may generate errors. But the build is successful.
