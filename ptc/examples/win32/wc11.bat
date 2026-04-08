@echo off
echo:

rem *****************************************************
rem *    Watcom C++ 11.0 PTC win32 gui example build    *
rem * ------------------------------------------------- *
rem * Syntax: wc11 [example] (library)                  *
rem *                                                   *
rem * [example]  = example source name                  *
rem * (library)  = "debug" or "release"                 *
rem *                                                   *                                           
rem * NOTE: to compile the examples for the win32       *
rem * platform it is necessary to use the /binnt watcom *
rem * binaries. this is because the dos binaries have a *
rem * limit on the size of command line arguments.      *
rem *****************************************************

if not exist "%1.cpp" goto done




rem ******************
rem * Win32 platform *
rem ******************
:dos

if "%2" == "debug" goto debug

echo Building "%1.cpp" : Watcom C++ 11.0 win32 release
echo:
rc -r windows.rc
wpp386 %1.cpp -i=..\..\source -bm -bt=nt -oneatxhl+ -5r -fp5 -fpi87
wlink name %1.exe file %1.obj library ..\..\library\win32\wc11.0\release.lib system nt_win option stack=256k, resource=windows.res
goto done

:debug
echo Building "%1.cpp" : Watcom C++ 11.0 win32 debug
echo:
rc -r windows.rc
wpp386 %1.cpp -i=..\..\source -bm -bt=nt
wlink name %1.exe file %1.obj library ..\..\library\win32\wc11.0\debug.lib system nt_win option stack=256k, resource=windows.res
goto done




:done
