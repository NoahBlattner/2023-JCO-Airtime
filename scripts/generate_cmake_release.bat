:: Generate CMake release build

:: Set path
set PATH=%PATH%;C:\Qt\Tools\CMake_64\bin
set PATH=%PATH%;C:/Qt/Tools/mingw1120_64/bin

:: Create build directory
cd..
mkdir cmake-build-release
cd cmake-build-release

:: Start build + static link
cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-static-libgcc -static-libstdc++ -static"
cmake --build .

:: Install
cmake --install .

:: Commands may generate errors. But the build is successful.
