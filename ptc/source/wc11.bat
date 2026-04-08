@echo off
rem *************************************
rem * Watcom C++ 11.0 PTC build         *
rem * --------------------------------- *
rem * Syntax: wc11 (platform) (library) *
rem *                                   *
rem * (platform) = "dos" or "win32"     *
rem * (library)  = "debug" or "release" *
rem *************************************

echo:
wmake -u -f wc11.mak PLATFORM=%1 LIBRARY=%2 %3 %4 %5 %6 %7 %8 %9
echo:
