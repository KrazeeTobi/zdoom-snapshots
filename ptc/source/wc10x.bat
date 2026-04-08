@echo off
rem **************************************
rem * Watcom C++ 10.5/10.6 PTC build     *
rem * ---------------------------------- *
rem * Syntax: wc10x (platform) (library) *
rem *                                    *
rem * (platform) = "dos" or "win32"      *
rem * (library)  = "debug" or "release"  *
rem **************************************

echo:
wmake -u -f wc10x.mak PLATFORM=%1 LIBRARY=%2 %3 %4 %5 %6 %7 %8 %9
echo:
