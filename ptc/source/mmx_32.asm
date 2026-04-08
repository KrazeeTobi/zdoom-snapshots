;----------------------------------------------------------------------------------------------------------------------------------------
; 32bit -> X pixel format conversions routines for PTC (intel MMX)
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

GLOBAL _Convert32_ABGR8888_MMX
GLOBAL _Convert32_RGBA8888_MMX
GLOBAL _Convert32_BGRA8888_MMX
GLOBAL _Convert32_RGB888_MMX
GLOBAL _Convert32_BGR888_MMX
GLOBAL _Convert32_RGB565_MMX
GLOBAL _Convert32_BGR565_MMX
GLOBAL _Convert32_ARGB1555_MMX
GLOBAL _Convert32_ABGR1555_MMX
GLOBAL _Convert32_GREY8_MMX
GLOBAL _Convert32_RGB332_MMX

SECTION .text





        




_Convert32_ABGR8888_MMX:                

    ret






_Convert32_RGBA8888_MMX:                

    ret






_Convert32_RGB888_MMX:

    ret






_Convert32_BGR888_MMX:

    ret






_Convert32_RGB565_MMX:                

    ret






_Convert32_BGR565_MMX:                

    ret






_Convert32_ARGB1555_MMX:                 

    ret






_Convert32_ABGR1555_MMX:                

    ret






_Convert32_GREY8_MMX:

    ret






_Convert32_RGB332_MMX:

    ret