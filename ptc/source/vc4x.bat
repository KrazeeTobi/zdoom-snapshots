@echo off
rem *************************************
rem * Visual C++ 4.x PTC build          *
rem * --------------------------------- *
rem * Syntax: vc4x (library)            *
rem *                                   *
rem * (library)  = "debug" or "release" *
rem *************************************

echo:
nmake /NOLOGO /S /f vc4x.mak CFG=%1 %2 %3 %4 %5 %6 %7 %8 %9
echo:
