;----------------------------------------------------------------------------------------------------------------------------------------
; X -> FAKEMODE pixel format conversions routines for PTC (intel MMX) 
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = source offset              
; edi = destination offset         
; ecx = number of pixels to convert
; ebx = extra data (only required for indexed modes)
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,edx                      
;----------------------------------------------------------------------------------------------------------------------------------------

BITS 32

GLOBAL _Convert32_FAKEMODE1A_MMX
GLOBAL _Convert32_FAKEMODE1B_MMX
GLOBAL _Convert32_FAKEMODE1C_MMX
GLOBAL _Convert32_FAKEMODE2A_MMX
GLOBAL _Convert32_FAKEMODE2B_MMX
GLOBAL _Convert32_FAKEMODE2C_MMX
GLOBAL _Convert32_FAKEMODE3A_MMX
GLOBAL _Convert32_FAKEMODE3B_MMX
GLOBAL _Convert32_FAKEMODE3C_MMX
GLOBAL _Convert16_FAKEMODE1A_MMX
GLOBAL _Convert16_FAKEMODE1B_MMX
GLOBAL _Convert16_FAKEMODE1C_MMX
GLOBAL _Convert16_FAKEMODE2A_MMX
GLOBAL _Convert16_FAKEMODE2B_MMX
GLOBAL _Convert16_FAKEMODE2C_MMX
GLOBAL _Convert16_FAKEMODE3A_MMX
GLOBAL _Convert16_FAKEMODE3B_MMX
GLOBAL _Convert16_FAKEMODE3C_MMX
GLOBAL _Convert8_FAKEMODE1A_MMX
GLOBAL _Convert8_FAKEMODE1B_MMX
GLOBAL _Convert8_FAKEMODE1C_MMX
GLOBAL _Convert8_FAKEMODE2A_MMX
GLOBAL _Convert8_FAKEMODE2B_MMX
GLOBAL _Convert8_FAKEMODE2C_MMX
GLOBAL _Convert8_FAKEMODE3A_MMX
GLOBAL _Convert8_FAKEMODE3B_MMX
GLOBAL _Convert8_FAKEMODE3C_MMX

SECTION .text










_Convert32_FAKEMODE1A_MMX:

    ret







_Convert32_FAKEMODE1B_MMX:

    ret







_Convert32_FAKEMODE1C_MMX:

    ret








_Convert32_FAKEMODE2A_MMX:

    ret







_Convert32_FAKEMODE2B_MMX:

    ret







_Convert32_FAKEMODE2C_MMX:

    ret









_Convert32_FAKEMODE3A_MMX:

    ret







_Convert32_FAKEMODE3B_MMX:

    ret







_Convert32_FAKEMODE3C_MMX:

    ret







_Convert16_FAKEMODE1A_MMX:

    ret







_Convert16_FAKEMODE1B_MMX:

    ret







_Convert16_FAKEMODE1C_MMX:

    ret







_Convert16_FAKEMODE2A_MMX:

    ret







_Convert16_FAKEMODE2B_MMX:

    ret







_Convert16_FAKEMODE2C_MMX:

    ret







_Convert16_FAKEMODE3A_MMX:

    ret







_Convert16_FAKEMODE3B_MMX:

    ret







_Convert16_FAKEMODE3C_MMX:

    ret








_Convert8_FAKEMODE1A_MMX:

    ret







_Convert8_FAKEMODE1B_MMX:

    ret







_Convert8_FAKEMODE1C_MMX:

    ret







_Convert8_FAKEMODE2A_MMX:

    ret







_Convert8_FAKEMODE2B_MMX:

    ret







_Convert8_FAKEMODE2C_MMX:

    ret







_Convert8_FAKEMODE3A_MMX:

    ret







_Convert8_FAKEMODE3B_MMX:

    ret







_Convert8_FAKEMODE3C_MMX:

    ret
