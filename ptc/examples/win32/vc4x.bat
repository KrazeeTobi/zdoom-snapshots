@echo off
echo:

rem *****************************************************
rem *    Visual C++ 4.x PTC win32 gui example build     *
rem * ------------------------------------------------- *
rem * Syntax: vc4x [example] (library)                  *
rem *                                                   *
rem * [example]  = example source name                  *
rem * (library)  = "debug" or "release"                 *
rem *****************************************************

if not exist "%1.cpp" goto done




rem ******************
rem * Win32 platform *
rem ******************

if "%2" == "debug" goto debug

echo Building "%1.cpp" : Visual C++ 4.x win32 release
echo:
rc -r windows.rc
cl %1.cpp windows.res /I..\..\source -MT -O2 -Ox ..\..\library\win32\vc4.x\release.lib kernel32.lib user32.lib gdi32.lib -D "WIN32" -D "NDEBUG" -D "_WINDOWS" -D "_MT" -nologo
goto done

:debug
echo Building "%1.cpp" : Visual C++ 4.x win32 debug
echo:
rc -r windows.rc
cl %1.cpp windows.res /I..\..\source -MTd -GX ..\..\library\win32\vc4.x\debug.lib kernel32.lib user32.lib gdi32.lib -D "WIN32" -D "_DEBUG" -D "_WINDOWS" -D "_MT" -nologo
goto done




:done
echo:
