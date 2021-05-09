@echo off

REM Replace with your Visual Studio installation directory
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86

msbuild /p:Configuration=Release /p:Platform=Win32 /m /t:Clean,Build
