;----------------------------------------------------------------------------------------------------------------------------------------
; 8bit -> X pixel format conversions routines for PTC (intel MMX)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = source offset              
; edi = destination offset         
; ecx = number of pixels to convert
; ebx = pointer to extra data (256 color index table)
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,edx                      
;----------------------------------------------------------------------------------------------------------------------------------------

BITS 32

GLOBAL _Convert8_4BYTE_MMX
GLOBAL _Convert8_3BYTE_MMX
GLOBAL _Convert8_2BYTE_MMX
GLOBAL _Convert8_1BYTE_MMX

SECTION .text










_Convert8_4BYTE_MMX:

    ret






_Convert8_3BYTE_MMX:

    ret






_Convert8_2BYTE_MMX:

    ret






_Convert8_1BYTE_MMX:

    ret
