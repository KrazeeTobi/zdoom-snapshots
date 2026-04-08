@echo off
rem *************************************
rem * DJGPP 2.x PTC build               *
rem * --------------------------------- *
rem * Syntax: djgpp (library)           *
rem *                                   *
rem * (library)  = "debug" or "release" *
rem *************************************

echo:
make -f djgpp.mak LIBRARY=%1 %2 %3 %4 %5 %6 %7 %8 %9
echo:
