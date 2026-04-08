;----------------------------------------------------------------------------------------------------------------------------------------
; Lookup tables for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; These tables are to be must be initialized by the library before first use
;----------------------------------------------------------------------------------------------------------------------------------------

GLOBAL _Convert16_ARGB8888_LUT_X86
GLOBAL _Convert16_ABGR8888_LUT_X86
GLOBAL _Convert16_RGBA8888_LUT_X86
GLOBAL _Convert16_BGRA8888_LUT_X86

SECTION .bss

_Convert16_ARGB8888_LUT_X86: RESD 512
_Convert16_ABGR8888_LUT_X86: RESD 512
_Convert16_RGBA8888_LUT_X86: RESD 512
_Convert16_BGRA8888_LUT_X86: RESD 512
