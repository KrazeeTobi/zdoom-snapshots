@echo off
echo:

rem *************************************
rem *    DJGPP 2.x PTC example build    *
rem * ----------------------------------*
rem * Syntax: djgpp [example] (library) *
rem *                                   *
rem * [example]  = example source name  *
rem * (library)  = "debug" or "release" *
rem *************************************

if not exist "%1.cpp" goto done




rem ****************
rem * Dos platform *
rem ****************

if "%2" == "debug" goto debug

echo Building "%1.cpp" : DJGPP 2.x dos release
echo:
gxx -o %1.exe %1.cpp ../../library/dos/djgpp/release.lib -I. -I../../source -I- -O3
goto done

:debug
echo Building "%1.cpp" : DJGPP 2.x dos debug
echo:
gxx -o %1.exe %1.cpp ../../library/dos/djgpp/debug.lib -I. -I../../source -I-
goto done




:done

