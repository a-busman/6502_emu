call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64

set TARGET=main
set CPLUSPLUSSRC=main.cpp cpu.cpp
set SRC=%CPLUSPLUSSRC%

del %TARGET%.exe

cl /EHsc %SRC%

del *.obj
