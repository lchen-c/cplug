@echo off

set /p type=[*] choose build type [1]Debug, [2]Release: 

set typestr=0
if "%type%"=="1" (
	set typestr=Debug
)else if "%type%"=="2" (
	set typestr=Release
)

cd /d %~dp0/build
cmake -DCMAKE_BUILD_TYPE="%typestr%" ..
set BUILD_CMD=cmake --build . --config %typestr% -j8
%BUILD_CMD%

cmake --install .
pause