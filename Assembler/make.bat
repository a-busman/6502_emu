@echo off
if "%ONCE%" NEQ "1" call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
set ONCE=1
set TARGET=main
set CPLUSPLUSSRC=main.cpp parser.cpp
set SRC=%CPLUSPLUSSRC%

del %TARGET%.exe

cl /EHsc %SRC%

del *.obj
