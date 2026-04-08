@echo off
echo:

rem *******************************************************
rem *         Cygnus GNU-Win32 PTC example build          *
rem * --------------------------------------------------- *
rem * Syntax: cygnus [example] (library)                  *
rem *                                                     *
rem * [example]  = example source name                    *
rem * (library)  = "debug" or "release"                   *
rem *                                                     *
rem * note: ignore the "cannot find entry symbol" warning *
rem *******************************************************

if not exist "%1.cpp" goto done




rem ******************
rem * Win32 platform *
rem ******************

if "%2" == "debug" goto debug

echo Building "%1.cpp" : Cygnus GNU-Win32 release
echo:
g++ %1.cpp -o %1.exe -Wl,--subsystem,windows ../../library/win32/cygnus/release.lib -I../../source -I- -lgdi32 -luser32 -O2
goto done

:debug
echo Building "%1.cpp" : Cygnus GNU-Win32 debug
echo:
g++ %1.cpp -o %1.exe -Wl,--subsystem,windows ../../library/win32/cygnus/debug.lib -I../../source -I- -lgdi32 -luser32
goto done





:done
echo:
