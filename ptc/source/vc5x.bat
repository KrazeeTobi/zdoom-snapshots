@echo off
rem *************************************
rem * Visual C++ 5.x PTC build          *
rem * --------------------------------- *
rem * Syntax: vc5x (library)            *
rem *                                   *
rem * (library)  = "debug" or "release" *
rem *************************************

echo:
nmake /NOLOGO /S /f vc5x.mak CFG=%1 %2 %3 %4 %5 %6 %7 %8 %9
echo:
