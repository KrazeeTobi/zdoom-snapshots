;----------------------------------------------------------------------------------------------------------------------------------------
; Lookup tables for PTC (Cygnus GNU-Win32 compiler / Intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; These tables are pre-initialized because Cygnus seems to dislike uninitialized data
; in the external assembly being manipulated inside C++ code. very strange :(
;----------------------------------------------------------------------------------------------------------------------------------------

GLOBAL _Convert16_ARGB8888_LUT_X86
GLOBAL _Convert16_ABGR8888_LUT_X86
GLOBAL _Convert16_RGBA8888_LUT_X86
GLOBAL _Convert16_BGRA8888_LUT_X86

SECTION .text

_Convert16_ARGB8888_LUT_X86: INCBIN "table1.dat"
_Convert16_ABGR8888_LUT_X86: INCBIN "table2.dat"
_Convert16_RGBA8888_LUT_X86: INCBIN "table3.dat"
_Convert16_BGRA8888_LUT_X86: INCBIN "table4.dat"
