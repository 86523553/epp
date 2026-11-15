@echo off
setlocal
pushd "%~dp0"

rem ── epp / 易屁屁 一键编译 ─────────────────────────────────────
rem 依赖：MinGW-w64 g++（x86_64，建议 GCC 12 以上）
rem 用法：① 已把 mingw64\bin 加进 PATH  → 直接双击本文件
rem       ② 没加 PATH                  → 改下面这行成你的绝对路径
rem          set "GPP=C:\mingw64\bin\g++.exe"
rem 产物：bin\ide.exe（静态链接，不依赖任何 DLL）

if not defined GPP set "GPP=g++.exe"
set "OUT=%TEMP%\epp_ide_build.exe"
if exist "%OUT%" del /q "%OUT%"

echo [1/3] 编译中（约 40-90 秒）...
"%GPP%" -std=c++17 -O2 -municode -mwindows -static -static-libgcc -static-libstdc++ "src\main.cpp" -o "%OUT%" -lgdiplus -lgdi32 -luser32 -lole32 -ladvapi32 -lshell32 -lcomdlg32 -lmsimg32 -limm32 -lwinmm
if errorlevel 1 (
  echo [FAIL] 编译失败：检查 GPP 路径与 MinGW 是否 64 位
  popd
  exit /b 1
)

echo [2/3] 拷贝到 bin...
if not exist "bin" mkdir "bin"
copy /y "%OUT%" "bin\ide.exe" >nul
if errorlevel 1 (
  echo [FAIL] 拷贝失败：先关掉正在运行的 bin\ide.exe 再试
  popd
  exit /b 1
)

echo [3/3] 完成
for %%F in ("bin\ide.exe") do echo   产物 %%~fF ^(%%~zF 字节^)
popd
exit /b 0
