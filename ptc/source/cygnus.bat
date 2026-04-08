@echo off
rem **************************************
rem * Cygnus GNU-Win32 PTC build         *
rem * ---------------------------------- *
rem * Syntax: cygnus (library)           *
rem *                                    *
rem * (library)  = "debug" or "release"  *
rem **************************************

echo:
make -f cygnus.mak LIBRARY=%1 %2 %3 %4 %5 %6 %7 %8 %9
echo:
