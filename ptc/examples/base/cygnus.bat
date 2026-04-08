@echo off
echo:

rem **************************************
rem * Cygnus GNU-Win32 PTC example build *
rem * ---------------------------------- *
rem * Syntax: cygnus [example] (library) *
rem *                                    *
rem * [example]  = example source name   *
rem * (library)  = "debug" or "release"  *
rem **************************************

if not exist "%1.cpp" goto done




rem ******************
rem * Win32 platform *
rem ******************

if "%2" == "debug" goto debug

echo Building "%1.cpp" : Cygnus GNU-Win32 release
echo:
g++ -o %1.exe %1.cpp -I../../source -I- ../../library/win32/cygnus/release.lib -lgdi32 -luser32 -O2
goto done

:debug
echo Building "%1.cpp" : Cygnus GNU-Win32 debug
echo:
g++ -o %1.exe %1.cpp -I../../source -I- ../../library/win32/cygnus/debug.lib -lgdi32 -luser32
goto done




:done
