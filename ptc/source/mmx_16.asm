;----------------------------------------------------------------------------------------------------------------------------------------
; 16bit -> X pixel format conversions routines for PTC (intel MMX)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = source offset              
; edi = destination offset         
; ecx = number of pixels to convert
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,edx                      
;----------------------------------------------------------------------------------------------------------------------------------------

BITS 32

GLOBAL _Convert16_ARGB8888_MMX
GLOBAL _Convert16_ABGR8888_MMX
GLOBAL _Convert16_RGBA8888_MMX
GLOBAL _Convert16_BGRA8888_MMX
GLOBAL _Convert16_RGB888_MMX
GLOBAL _Convert16_BGR888_MMX
GLOBAL _Convert16_BGR565_MMX
GLOBAL _Convert16_ARGB1555_MMX
GLOBAL _Convert16_ABGR1555_MMX
GLOBAL _Convert16_GREY8_MMX
GLOBAL _Convert16_RGB332_MMX

SECTION .text









_Convert16_ARGB8888_MMX:

    ret






_Convert16_ABGR8888_MMX:

    ret






_Convert16_RGBA8888_MMX:

    ret






_Convert16_BGRA8888_MMX:

    ret






_Convert16_RGB888_MMX:

    ret





_Convert16_BGR888_MMX:

    ret






_Convert16_BGR565_MMX:

    ret






_Convert16_ARGB1555_MMX:

    ret






_Convert16_ABGR1555_MMX:

    ret






_Convert16_GREY8_MMX:

    ret






_Convert16_RGB332_MMX:

    ret





