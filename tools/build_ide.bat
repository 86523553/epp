@echo off
rem 统一编译入口：编译源码/main.cpp 到 %TEMP%\ide_build.exe，错误写 %TEMP%\ide_build_err.txt
pushd "%~dp0..\源码"
set GPP=C:\mingw64\bin\g++.exe
set OUT=%TEMP%\ide_build.exe
set ERR=%TEMP%\ide_build_err.txt
if exist "%OUT%" del /q "%OUT%"
"%GPP%" -std=c++17 -O2 -municode -mwindows -static -static-libgcc -static-libstdc++ main.cpp -o "%OUT%" -lgdiplus -lgdi32 -luser32 -lole32 -ladvapi32 -lshell32 -lcomdlg32 -lmsimg32 -limm32 -lwinmm > "%ERR%" 2>&1
echo EXIT=%ERRORLEVEL%
endlocal
