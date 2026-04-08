;----------------------------------------------------------------------------------------------------------------------------------------
; X -> FAKEMODE pixel format conversions routines for PTC (intel x86) 
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

GLOBAL _Convert32_FAKEMODE1A_X86
GLOBAL _Convert32_FAKEMODE1B_X86
GLOBAL _Convert32_FAKEMODE1C_X86
GLOBAL _Convert32_FAKEMODE2A_X86
GLOBAL _Convert32_FAKEMODE2B_X86
GLOBAL _Convert32_FAKEMODE2C_X86
GLOBAL _Convert32_FAKEMODE3A_X86 
GLOBAL _Convert32_FAKEMODE3B_X86
GLOBAL _Convert32_FAKEMODE3C_X86
GLOBAL _Convert16_FAKEMODE1A_X86
GLOBAL _Convert16_FAKEMODE1B_X86
GLOBAL _Convert16_FAKEMODE1C_X86
GLOBAL _Convert16_FAKEMODE2A_X86
GLOBAL _Convert16_FAKEMODE2B_X86
GLOBAL _Convert16_FAKEMODE2C_X86
GLOBAL _Convert16_FAKEMODE3A_X86
GLOBAL _Convert16_FAKEMODE3B_X86
GLOBAL _Convert16_FAKEMODE3C_X86
GLOBAL _Convert8_FAKEMODE1A_X86
GLOBAL _Convert8_FAKEMODE1B_X86
GLOBAL _Convert8_FAKEMODE1C_X86
GLOBAL _Convert8_FAKEMODE2A_X86
GLOBAL _Convert8_FAKEMODE2B_X86
GLOBAL _Convert8_FAKEMODE2C_X86
GLOBAL _Convert8_FAKEMODE3A_X86
GLOBAL _Convert8_FAKEMODE3B_X86
GLOBAL _Convert8_FAKEMODE3C_X86

SECTION .text






_Convert32_FAKEMODE1A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RGB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RGB
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi    

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert32_FAKEMODE1B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt32_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert32_FAKEMODE1C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret









_Convert32_FAKEMODE2A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG
   
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert32_FAKEMODE2B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt32_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt32_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert32_FAKEMODE2C_X86:
                     
    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_GBR

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_RBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE2_GBR
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*8
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret









_Convert32_FAKEMODE3A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RGBRGB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RGBRGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RGBRGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RGBRGB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RGBRGB
    call PlaneBlt32_FAKEMODE3_RGBR

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RGBRGB
    call PlaneBlt32_FAKEMODE3_RGBR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,8
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RGBRGB
    call PlaneBlt32_FAKEMODE3_RGBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3*4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RGBRGB
    call PlaneBlt32_FAKEMODE3_RGBR

    ret







_Convert32_FAKEMODE3B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_GRBRBG

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RBGGRB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_GRBRBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RBGGRB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_GRBRBG
    call PlaneBlt32_FAKEMODE3_GRBR

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RBGGRB
    call PlaneBlt32_FAKEMODE3_RBGG

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,8
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_GRBRBG
    call PlaneBlt32_FAKEMODE3_GRBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3*4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RBGGRB
    call PlaneBlt32_FAKEMODE3_RBGG

    ret







_Convert32_FAKEMODE3C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_GRBGRB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RBGRBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,8
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_GRBGRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3*4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt32_FAKEMODE3_RBGRBG

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_GRBGRB
    call PlaneBlt32_FAKEMODE3_GRBG

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RBGRBG
    call PlaneBlt32_FAKEMODE3_RBGR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,8
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_GRBGRB
    call PlaneBlt32_FAKEMODE3_GRBG

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3*4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt32_FAKEMODE3_RBGRBG
    call PlaneBlt32_FAKEMODE3_RBGR

    ret












_Convert16_FAKEMODE1A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RGB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RGB
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi    

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE1B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt16_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE1C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE2A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE2B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1    
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt16_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt16_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE2C_X86:
                     
    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_GBR

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_RBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE2_GBR
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*4
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert16_FAKEMODE3A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RGBRGB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RGBRGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RGBRGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RGBRGB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RGBRGB
    call PlaneBlt16_FAKEMODE3_RGBR

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RGBRGB
    call PlaneBlt16_FAKEMODE3_RGBR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RGBRGB
    call PlaneBlt16_FAKEMODE3_RGBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,6
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RGBRGB
    call PlaneBlt16_FAKEMODE3_RGBR

    ret







_Convert16_FAKEMODE3B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_GRBRBG

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RBGGRB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_GRBRBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RBGGRB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_GRBRBG
    call PlaneBlt16_FAKEMODE3_GRBR

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RBGGRB
    call PlaneBlt16_FAKEMODE3_RBGG

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_GRBRBG
    call PlaneBlt16_FAKEMODE3_GRBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,6
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RBGGRB
    call PlaneBlt16_FAKEMODE3_RBGG

    ret







_Convert16_FAKEMODE3C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_GRBGRB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RBGRBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,4
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_GRBGRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,6
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt16_FAKEMODE3_RBGRBG

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_GRBGRB
    call PlaneBlt16_FAKEMODE3_GRBG

    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RBGRBG
    call PlaneBlt16_FAKEMODE3_RBGR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,4
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_GRBGRB
    call PlaneBlt16_FAKEMODE3_GRBG

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,6
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt16_FAKEMODE3_RBGRBG
    call PlaneBlt16_FAKEMODE3_RBGR

    ret












_Convert8_FAKEMODE1A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RGB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RGB
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi    

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE1B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE1_GRB
        mov edx,1
        call PlaneBlt8_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE1C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_GRB
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_GRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE1_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE2A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE2B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1    
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,1
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
        mov edx,1
        call PlaneBlt8_FAKEMODE2_GBR
        mov edx,1
        call PlaneBlt8_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE2C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 25 x 8 = 200
    mov ecx,25

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_GBR
    
        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_GBR

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,8
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE2_RBG
    
        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*2
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







_Convert8_FAKEMODE3A_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RGBRGB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RGBRGB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RGBRGB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RGBRGB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RGBRGB
    call PlaneBlt8_FAKEMODE3_RGBR
                 
    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,1
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RGBRGB
    call PlaneBlt8_FAKEMODE3_RGBR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RGBRGB
    call PlaneBlt8_FAKEMODE3_RGBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RGBRGB
    call PlaneBlt8_FAKEMODE3_RGBR

    ret







_Convert8_FAKEMODE3B_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_GRBRBG

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RBGGRB

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_GRBRBG

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RBGGRB

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_GRBRBG
    call PlaneBlt8_FAKEMODE3_GRBR
                 
    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,1
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RBGGRB
    call PlaneBlt8_FAKEMODE3_RBGG

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_GRBRBG
    call PlaneBlt8_FAKEMODE3_GRBR

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RBGGRB
    call PlaneBlt8_FAKEMODE3_RBGG

    ret







_Convert8_FAKEMODE3C_X86:

    mov [DestOffset],edi
    mov [SourceOffset],esi

    ; 16 x 12 = 192 (8 lines leftover)
    mov ecx,16

.L1     push ecx

        ; plane zero
        mov esi,[SourceOffset]
        mov eax,0102h
        mov edx,03C4h
        out dx,ax
        mov edx,4                                              ; 4 x 3 = 12 lines at a time
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_GRBGRB

        ; plane one
        mov esi,[SourceOffset]
        mov eax,0202h
        add esi,1
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RBGRBG

        ; plane two
        mov esi,[SourceOffset]
        mov eax,0402h
        add esi,2
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_GRBGRB

        ; plane three
        mov esi,[SourceOffset]
        mov eax,0802h
        add esi,3
        mov edx,03C4h
        out dx,ax
        mov edx,4
        mov edi,[DestOffset]
        call PlaneBlt8_FAKEMODE3_RBGRBG

        mov [DestOffset],edi
        mov esi,[SourceOffset]
        add esi,320*4*3
        mov [SourceOffset],esi

        pop ecx

        dec ecx
        jz .L2
        jmp .L1

.L2 ; plane zero
    mov esi,[SourceOffset]
    mov eax,0102h
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_GRBGRB
    call PlaneBlt8_FAKEMODE3_GRBG
                 
    ; plane one
    mov esi,[SourceOffset]
    mov eax,0202h
    add esi,1
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RBGRBG
    call PlaneBlt8_FAKEMODE3_RBGR

    ; plane two
    mov esi,[SourceOffset]
    mov eax,0402h
    add esi,2
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_GRBGRB
    call PlaneBlt8_FAKEMODE3_GRBG

    ; plane three
    mov esi,[SourceOffset]
    mov eax,0802h
    add esi,3
    mov edx,03C4h
    out dx,ax
    mov edx,2
    mov edi,[DestOffset]
    call PlaneBlt8_FAKEMODE3_RBGRBG
    call PlaneBlt8_FAKEMODE3_RBGR

    ret



















;----------------------------------------------------------------------------------------------------------------------------------------
; X -> FAKEMODE planar pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = source offset              
; edi = destination offset         
; edx = number of lines
; ebx = extra data (only required for indexed modes)
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ecx                      
;----------------------------------------------------------------------------------------------------------------------------------------







PlaneBlt32_FAKEMODE1_RGB:

    ; save ebp
    push ebp
    mov ebp,edx

.L1     mov ecx,20

.L2         mov dl,[esi+0]                                  ; blues
            mov bl,[esi+1]                                  ; greens

            mov dh,[esi+16]

            shl edx,16
            mov al,[esi+2]                                  ; reds

            mov bh,[esi+16+1]
            mov ah,[esi+16+2]

            shl ebx,16
            mov dl,[esi+16+16]

            shl eax,16
            mov dh,[esi+16+16+16]

            mov bl,[esi+16+16+1]
            and edx,11111100111111001111110011111100b

            ror edx,18
            mov al,[esi+16+16+2]

            add edx,80808080h
            mov bh,[esi+16+16+16+1]

            mov [edi+160],edx
            mov ah,[esi+16+16+16+2]

            and ebx,11111100111111001111110011111100b
            and eax,11111100111111001111110011111100b

            ror ebx,18
            add edi,4

            add esi,4*4*4
            add ebx,40404040h

            ror eax,18
            mov [edi+80-4],ebx

            mov [edi-4],eax
            dec ecx

            jnz .L2

        add edi,160
        dec ebp
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt32_FAKEMODE1_RBG:

    ; save ebp
    push ebp
    mov ebp,edx

.L1     mov ecx,20

.L2         mov dl,[esi+0]                                  ; blues
            mov bl,[esi+1]                                  ; greens

            mov dh,[esi+16]

            shl edx,16
            mov al,[esi+2]                                  ; reds

            mov bh,[esi+16+1]
            mov ah,[esi+16+2]

            shl ebx,16
            mov dl,[esi+16+16]

            shl eax,16
            mov dh,[esi+16+16+16]

            mov bl,[esi+16+16+1]
            and edx,11111100111111001111110011111100b

            ror edx,18
            mov al,[esi+16+16+2]

            add edx,80808080h
            mov bh,[esi+16+16+16+1]

            mov [edi+80],edx
            mov ah,[esi+16+16+16+2]

            and ebx,11111100111111001111110011111100b
            and eax,11111100111111001111110011111100b

            ror ebx,18
            add edi,4

            add esi,4*4*4
            add ebx,40404040h

            ror eax,18
            mov [edi+160-4],ebx

            mov [edi-4],eax
            dec ecx

            jnz .L2

        add edi,160
        dec ebp
        jnz .L1

.L3 pop ebp
    ret








PlaneBlt32_FAKEMODE1_GRB:

    ; save ebp
    push ebp
    mov ebp,edx

.L1     mov ecx,20

.L2         mov dl,[esi+0]                                  ; blues
            mov bl,[esi+1]                                  ; greens

            mov dh,[esi+16]

            shl edx,16
            mov al,[esi+2]                                  ; reds

            mov bh,[esi+16+1]
            mov ah,[esi+16+2]

            shl ebx,16
            mov dl,[esi+16+16]

            shl eax,16
            mov dh,[esi+16+16+16]

            mov bl,[esi+16+16+1]
            and edx,11111100111111001111110011111100b

            ror edx,18
            mov al,[esi+16+16+2]

            add edx,80808080h
            mov bh,[esi+16+16+16+1]

            mov [edi+160],edx
            mov ah,[esi+16+16+16+2]

            and ebx,11111100111111001111110011111100b
            and eax,11111100111111001111110011111100b

            ror ebx,18
            add edi,4

            add esi,4*4*4
            add ebx,40404040h

            ror eax,18
            mov [edi-4],ebx

            mov [edi+80-4],eax
            dec ecx

            jnz .L2

        add edi,160
        dec ebp
        jnz .L1

.L3 pop ebp
    ret








PlaneBlt32_FAKEMODE2_RBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov dl,[esi+1]                                  ; greens
            mov dh,[esi+16+1]
            shl edx,16
            mov dl,[esi+16+16+1]
            mov dh,[esi+16+16+16+1]
            and edx,11111000111110001111100011111000b
            ror edx,19

            mov bl,[esi+0]                                  ; blues
            mov bh,[esi+16]
            shl ebx,16
            mov bl,[esi+16+16]
            mov bh,[esi+16+16+16]
            and ebx,11100000111000001110000011100000b
            ror ebx,21

            mov al,[esi+2]                                  ; reds
            mov ah,[esi+16+2]
            shl eax,16
            mov al,[esi+16+16+2]
            mov ah,[esi+16+16+16+2]
            mov ecx,eax 

            and eax,11110000111100001111000011110000b       ; top reds
            ror eax,17

            and ecx,11000000110000001100000011000000b       ; bottom reds
            ror ecx,17
            or  ecx,10000000100000001000000010000000b

            add eax,ebx                                     ; combine
            add ecx,edx 

            mov [edi],eax
            mov [edi+80],ecx

            add esi,4*4*4
            add edi,4

            dec ebp
            jnz .L2

        pop edx
        add edi,80

        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt32_FAKEMODE2_GBR:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov dl,[esi+1]                                  ; greens
            mov dh,[esi+16+1]
            shl edx,16
            mov dl,[esi+16+16+1]
            mov dh,[esi+16+16+16+1]
            and edx,11111000111110001111100011111000b
            ror edx,19

            mov bl,[esi+0]                                  ; blues
            mov bh,[esi+16]
            shl ebx,16
            mov bl,[esi+16+16]
            mov bh,[esi+16+16+16]
            and ebx,11100000111000001110000011100000b
            ror ebx,21

            mov al,[esi+2]                                  ; reds
            mov ah,[esi+16+2]
            shl eax,16
            mov al,[esi+16+16+2]
            mov ah,[esi+16+16+16+2]
            mov ecx,eax 

            and eax,11110000111100001111000011110000b       ; top reds
            ror eax,17

            and ecx,11000000110000001100000011000000b       ; bottom reds
            ror ecx,17
            or  ecx,10000000100000001000000010000000b

            add eax,ebx                                     ; combine
            add ecx,edx 

            mov [edi+80],eax
            mov [edi],ecx

            add esi,4*4*4
            add edi,4

            dec ebp
            jnz .L2

        pop edx
        add edi,80
        
        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt32_FAKEMODE3_RGBRGB:

    push ebp
    mov ebp,edx
    
.L1     mov ecx,20

.L2         ; first RGB line [0r,0g,1b]
 
            mov al,[esi+16+16+2]                            ; eax = 0r
            mov ah,[esi+16+16+16+2]
            shl eax,16
            mov ah,[esi+16+2]
            mov al,[esi+2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1]                         ; ebx = 0g
            mov bl,[esi+16+16+1]
            shl ebx,16
            mov bh,[esi+16+1]
            mov bl,[esi+1]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4]                        ; edx = 1b
            mov dh,[esi+16+16+16+320*4]                                   
            shl edx,16
            mov dl,[esi+0+320*4]                     
            mov dh,[esi+16+320*4]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h
 
            mov [edi],eax      ; 0r
            mov [edi+80],ebx   ; 0g
            mov [edi+160],edx  ; 1b

            ; second RGB line [1r,2g,2b]
        
            mov al,[esi+16+16+2+320*4]                      ; eax = 1r            
            mov ah,[esi+16+16+16+2+320*4]
            shl eax,16
            mov al,[esi+2+320*4]                                        
            mov ah,[esi+16+2+320*4]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1+320*4*2]                 ; ebx = 2g
            mov bl,[esi+16+16+1+320*4*2]
            shl ebx,16
            mov bh,[esi+16+1+320*4*2]
            mov bl,[esi+1+320*4*2]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4*2]                      ; edx = 2b            
            mov dh,[esi+16+16+16+320*4*2]
            shl edx,16
            mov dl,[esi+0+320*4*2]                                      
            mov dh,[esi+16+320*4*2]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h

            mov [edi+240],eax      ; 1r
            mov [edi+240+80],ebx   ; 2g
            mov [edi+240+160],edx  ; 2b

            add esi,4*4*4
            add edi,4

            dec ecx
            jz .L3
            jmp .L2

.L3     add edi,320+80
        add esi,320*4*2
        dec ebp
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt32_FAKEMODE3_RBGRBG:

    push ebp
    mov ebp,edx
    
.L1     mov ecx,20

.L2         ; first RGB line [0r,0b,1g]
 
            mov al,[esi+16+16+2]                            ; eax = 0r
            mov ah,[esi+16+16+16+2]
            shl eax,16
            mov ah,[esi+16+2]
            mov al,[esi+2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov dl,[esi+16+16]                              ; edx = 0b
            mov dh,[esi+16+16+16]                                   
            shl edx,16
            mov dl,[esi]                     
            mov dh,[esi+16]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h
 
            mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
            mov bl,[esi+16+16+1+320*4]
            shl ebx,16
            mov bh,[esi+16+1+320*4]
            mov bl,[esi+1+320*4]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov [edi],eax      ; 0r
            mov [edi+80],edx   ; 0b
            mov [edi+160],ebx  ; 1g

            ; second RGB line [1r,2b,2g]
        
            mov al,[esi+16+16+2+320*4]                      ; eax = 1r            
            mov ah,[esi+16+16+16+2+320*4]
            shl eax,16
            mov al,[esi+2+320*4]                                        
            mov ah,[esi+16+2+320*4]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1+320*4*2]                 ; ebx = 2g
            mov bl,[esi+16+16+1+320*4*2]
            shl ebx,16
            mov bh,[esi+16+1+320*4*2]
            mov bl,[esi+1+320*4*2]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4*2]                      ; edx = 2b            
            mov dh,[esi+16+16+16+320*4*2]
            shl edx,16
            mov dl,[esi+0+320*4*2]                                      
            mov dh,[esi+16+320*4*2]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h

            mov [edi+240],eax      ; 1r
            mov [edi+240+80],edx   ; 2b
            mov [edi+240+160],ebx  ; 2g

            add esi,4*4*4
            add edi,4

            dec ecx
            jz .L3
            jmp .L2

.L3     add edi,320+80
        add esi,320*4*2
        dec ebp
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt32_FAKEMODE3_GRBGRB:

    push ebp
    mov ebp,edx
    
.L1     mov ecx,20

.L2         ; first RGB line [0g,0r,1b]
 
            mov al,[esi+16+16+2]                            ; eax = 0r
            mov ah,[esi+16+16+16+2]
            shl eax,16
            mov ah,[esi+16+2]
            mov al,[esi+2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1]                         ; ebx = 0g
            mov bl,[esi+16+16+1]
            shl ebx,16
            mov bh,[esi+16+1]
            mov bl,[esi+1]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4]                        ; edx = 1b
            mov dh,[esi+16+16+16+320*4]                                   
            shl edx,16
            mov dl,[esi+0+320*4]                     
            mov dh,[esi+16+320*4]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h
 
            mov [edi],ebx      ; 0g
            mov [edi+80],eax   ; 0r
            mov [edi+160],edx  ; 1b

            ; second RGB line [1g,2r,2b]
        
            mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
            mov bl,[esi+16+16+1+320*4]
            shl ebx,16
            mov bh,[esi+16+1+320*4]
            mov bl,[esi+1+320*4]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov al,[esi+16+16+2+320*4*2]                    ; eax = 2r            
            mov ah,[esi+16+16+16+2+320*4*2]
            shl eax,16
            mov al,[esi+2+320*4*2]                                        
            mov ah,[esi+16+2+320*4*2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov dl,[esi+16+16+320*4*2]                      ; edx = 2b            
            mov dh,[esi+16+16+16+320*4*2]
            shl edx,16
            mov dl,[esi+0+320*4*2]                                      
            mov dh,[esi+16+320*4*2]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h

            mov [edi+240],ebx      ; 1g
            mov [edi+240+80],eax   ; 2r
            mov [edi+240+160],edx  ; 2b

            add esi,4*4*4
            add edi,4

            dec ecx
            jz .L3
            jmp .L2

.L3     add edi,320+80
        add esi,320*4*2
        dec ebp
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt32_FAKEMODE3_RBGGRB:

    push ebp
    mov ebp,edx
    
.L1     mov ecx,20

.L2         ; first RGB line [0r,0b,1g]
 
            mov al,[esi+16+16+2]                            ; eax = 0r
            mov ah,[esi+16+16+16+2]
            shl eax,16
            mov ah,[esi+16+2]
            mov al,[esi+2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov dl,[esi+16+16]                              ; edx = 0b
            mov dh,[esi+16+16+16]                                   
            shl edx,16
            mov dl,[esi]                     
            mov dh,[esi+16]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h
 
            mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
            mov bl,[esi+16+16+1+320*4]
            shl ebx,16
            mov bh,[esi+16+1+320*4]
            mov bl,[esi+1+320*4]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov [edi],eax      ; 0r
            mov [edi+80],edx   ; 0b
            mov [edi+160],ebx  ; 1g

            ; second RGB line [1g,2r,2b]
        
            mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
            mov bl,[esi+16+16+1+320*4]
            shl ebx,16
            mov bh,[esi+16+1+320*4]
            mov bl,[esi+1+320*4]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov al,[esi+16+16+2+320*4*2]                    ; eax = 2r            
            mov ah,[esi+16+16+16+2+320*4*2]
            shl eax,16
            mov al,[esi+2+320*4*2]                                        
            mov ah,[esi+16+2+320*4*2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov dl,[esi+16+16+320*4*2]                      ; edx = 2b            
            mov dh,[esi+16+16+16+320*4*2]
            shl edx,16
            mov dl,[esi+0+320*4*2]                                      
            mov dh,[esi+16+320*4*2]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h

            mov [edi+240],ebx      ; 1g
            mov [edi+240+80],eax   ; 2r
            mov [edi+240+160],edx  ; 2b

            add esi,4*4*4
            add edi,4

            dec ecx
            jz .L3
            jmp .L2

.L3     add edi,320+80
        add esi,320*4*2
        dec ebp
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt32_FAKEMODE3_GRBRBG:

    push ebp
    mov ebp,edx
    
.L1     mov ecx,20

.L2         ; first RGB line [0g,0r,1b]
 
            mov al,[esi+16+16+2]                            ; eax = 0r
            mov ah,[esi+16+16+16+2]
            shl eax,16
            mov ah,[esi+16+2]
            mov al,[esi+2]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1]                         ; ebx = 0g
            mov bl,[esi+16+16+1]
            shl ebx,16
            mov bh,[esi+16+1]
            mov bl,[esi+1]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4]                        ; edx = 1b
            mov dh,[esi+16+16+16+320*4]                                   
            shl edx,16
            mov dl,[esi+0+320*4]                     
            mov dh,[esi+16+320*4]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h
 
            mov [edi],ebx      ; 0g
            mov [edi+80],eax   ; 0r
            mov [edi+160],edx  ; 1b

            ; second RGB line [1r,2b,2g]
        
            mov al,[esi+16+16+2+320*4]                      ; eax = 1r            
            mov ah,[esi+16+16+16+2+320*4]
            shl eax,16
            mov al,[esi+2+320*4]                                        
            mov ah,[esi+16+2+320*4]
            and eax,11111100111111001111110011111100b
            shr eax,2

            mov bh,[esi+16+16+16+1+320*4*2]                 ; ebx = 2g
            mov bl,[esi+16+16+1+320*4*2]
            shl ebx,16
            mov bh,[esi+16+1+320*4*2]
            mov bl,[esi+1+320*4*2]
            and ebx,11111100111111001111110011111100b
            shr ebx,2
            add ebx,40404040h

            mov dl,[esi+16+16+320*4*2]                      ; edx = 2b            
            mov dh,[esi+16+16+16+320*4*2]
            shl edx,16
            mov dl,[esi+0+320*4*2]                                      
            mov dh,[esi+16+320*4*2]
            and edx,11111100111111001111110011111100b
            shr edx,2
            add edx,80808080h

            mov [edi+240],eax      ; 1r
            mov [edi+240+80],edx   ; 2b
            mov [edi+240+160],ebx  ; 2g

            add esi,4*4*4
            add edi,4

            dec ecx
            jz .L3
            jmp .L2

.L3     add edi,320+80
        add esi,320*4*2
        dec ebp
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt32_FAKEMODE3_RGBR:

    mov ecx,20

.L1     mov al,[esi+16+16+2]                            ; eax = 0r
        mov ah,[esi+16+16+16+2]
        shl eax,16
        mov ah,[esi+16+2]
        mov al,[esi+2]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov bh,[esi+16+16+16+1]                         ; ebx = 0g
        mov bl,[esi+16+16+1]
        shl ebx,16
        mov bh,[esi+16+1]
        mov bl,[esi+1]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov [edi],eax      ; 0r
        mov [edi+80],ebx   ; 0g

        mov al,[esi+16+16+2+320*4]                      ; eax = 1r
        mov ah,[esi+16+16+16+2+320*4]
        shl eax,16
        mov ah,[esi+16+2+320*4]
        mov al,[esi+2+320*4]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov dl,[esi+16+16+320*4]                        ; edx = 1b
        mov dh,[esi+16+16+16+320*4]                                   
        shl edx,16
        mov dl,[esi+0+320*4]                     
        mov dh,[esi+16+320*4]
        and edx,11111100111111001111110011111100b
        shr edx,2
        add edx,80808080h
 
        mov [edi+160],edx  ; 1b
        mov [edi+240],eax  ; 1r

        add esi,4*4*4
        add edi,4

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







PlaneBlt32_FAKEMODE3_RBGR:

    mov ecx,20

.L1     mov al,[esi+16+16+2]                            ; eax = 0r
        mov ah,[esi+16+16+16+2]
        shl eax,16
        mov ah,[esi+16+2]
        mov al,[esi+2]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov dl,[esi+16+16]                              ; edx = 0b
        mov dh,[esi+16+16+16]                                   
        shl edx,16
        mov dl,[esi]                     
        mov dh,[esi+16]
        and edx,11111100111111001111110011111100b
        shr edx,2
        add edx,80808080h
 
        mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
        mov bl,[esi+16+16+1+320*4]
        shl ebx,16
        mov bh,[esi+16+1+320*4]
        mov bl,[esi+1+320*4]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov [edi],eax      ; 0r
        mov [edi+80],edx   ; 0b
        mov [edi+160],ebx  ; 1g

        mov al,[esi+16+16+2+320*4]                      ; eax = 1r            
        mov ah,[esi+16+16+16+2+320*4]
        shl eax,16
        mov al,[esi+2+320*4]                                        
        mov ah,[esi+16+2+320*4]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov [edi+240],eax  ; 1r

        add esi,4*4*4
        add edi,4

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







PlaneBlt32_FAKEMODE3_GRBG:

    mov ecx,20

.L1     mov al,[esi+16+16+2]                            ; eax = 0r
        mov ah,[esi+16+16+16+2]
        shl eax,16
        mov ah,[esi+16+2]
        mov al,[esi+2]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov bh,[esi+16+16+16+1]                         ; ebx = 0g
        mov bl,[esi+16+16+1]
        shl ebx,16
        mov bh,[esi+16+1]
        mov bl,[esi+1]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov dl,[esi+16+16+320*4]                        ; edx = 1b
        mov dh,[esi+16+16+16+320*4]                                   
        shl edx,16
        mov dl,[esi+0+320*4]                     
        mov dh,[esi+16+320*4]
        and edx,11111100111111001111110011111100b
        shr edx,2
        add edx,80808080h
 
        mov [edi],ebx      ; 0g
        mov [edi+80],eax   ; 0r
        mov [edi+160],edx  ; 1b

        mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
        mov bl,[esi+16+16+1+320*4]
        shl ebx,16
        mov bh,[esi+16+1+320*4]
        mov bl,[esi+1+320*4]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov [edi+240],ebx      ; 1g

        add esi,4*4*4
        add edi,4

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







PlaneBlt32_FAKEMODE3_RBGG:

    mov ecx,20

.L1     mov al,[esi+16+16+2]                            ; eax = 0r
        mov ah,[esi+16+16+16+2]
        shl eax,16
        mov ah,[esi+16+2]
        mov al,[esi+2]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov dl,[esi+16+16]                              ; edx = 0b
        mov dh,[esi+16+16+16]                                   
        shl edx,16
        mov dl,[esi]                     
        mov dh,[esi+16]
        and edx,11111100111111001111110011111100b
        shr edx,2
        add edx,80808080h
 
        mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
        mov bl,[esi+16+16+1+320*4]
        shl ebx,16
        mov bh,[esi+16+1+320*4]
        mov bl,[esi+1+320*4]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov [edi],eax      ; 0r
        mov [edi+80],edx   ; 0b
        mov [edi+160],ebx  ; 1g

        mov bh,[esi+16+16+16+1+320*4]                   ; ebx = 1g
        mov bl,[esi+16+16+1+320*4]
        shl ebx,16
        mov bh,[esi+16+1+320*4]
        mov bl,[esi+1+320*4]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov [edi+240],ebx      ; 1g

        add esi,4*4*4
        add edi,4

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







PlaneBlt32_FAKEMODE3_GRBR:

    mov ecx,20

.L1     mov al,[esi+16+16+2]                            ; eax = 0r
        mov ah,[esi+16+16+16+2]
        shl eax,16
        mov ah,[esi+16+2]
        mov al,[esi+2]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov bh,[esi+16+16+16+1]                         ; ebx = 0g
        mov bl,[esi+16+16+1]
        shl ebx,16
        mov bh,[esi+16+1]
        mov bl,[esi+1]
        and ebx,11111100111111001111110011111100b
        shr ebx,2
        add ebx,40404040h

        mov dl,[esi+16+16+320*4]                        ; edx = 1b
        mov dh,[esi+16+16+16+320*4]                                   
        shl edx,16
        mov dl,[esi+0+320*4]                     
        mov dh,[esi+16+320*4]
        and edx,11111100111111001111110011111100b
        shr edx,2
        add edx,80808080h
 
        mov [edi],ebx      ; 0g
        mov [edi+80],eax   ; 0r
        mov [edi+160],edx  ; 1b

        mov al,[esi+16+16+2+320*4]                      ; eax = 1r            
        mov ah,[esi+16+16+16+2+320*4]
        shl eax,16
        mov al,[esi+2+320*4]                                        
        mov ah,[esi+16+2+320*4]
        and eax,11111100111111001111110011111100b
        shr eax,2

        mov [edi+240],eax      ; 1r

        add esi,4*4*4
        add edi,4

        dec ecx
        jz .L2
        jmp .L1

.L2 ret







PlaneBlt16_FAKEMODE1_RGB:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi],eax
            mov [edi+80],ecx
            mov [edi+160],ebx

            add edi,4
            add esi,4*4*2

            dec ebp
            jnz .L2

        pop edx
        add edi,160

        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt16_FAKEMODE1_RBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi],eax
            mov [edi+160],ecx
            mov [edi+80],ebx

            add edi,4
            add esi,4*4*2

            dec ebp
            jnz .L2

        pop edx
        add edi,160

        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt16_FAKEMODE1_GRB:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+80],eax
            mov [edi],ecx
            mov [edi+160],ebx

            add edi,4
            add esi,4*4*2

            dec ebp
            jnz .L2

        pop edx
        add edi,160

        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt16_FAKEMODE2_RBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi]                                    ; greens (low 2 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11000000110000001100000011000000b
            ror ecx,16+6

            mov dl,[esi+1]                                  ; greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-2
   
            and ebx,00011100000111000001110000011100b       ; blues
            ror ebx,16+2

            and eax,11110000111100001111000011110000b       ; reds
            ror eax,16+1

            add eax,ebx                                     ; EAX = red+blue
            add ecx,edx                                     ; ECX = green(lo)+green(hi)

            or ecx,10000000100000001000000010000000b

            mov [edi],eax
            mov [edi+80],ecx

            add esi,4*4*2
            add edi,4

            dec ebp
            jnz .L2

        pop edx
        add edi,80

        dec edx
        jnz .L1

.L3 pop ebp
    ret






PlaneBlt16_FAKEMODE2_GBR:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi]                                    ; greens (low 2 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11000000110000001100000011000000b
            ror ecx,16+6

            mov dl,[esi+1]                                  ; greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-2
   
            and ebx,00011100000111000001110000011100b       ; blues
            ror ebx,16+2

            and eax,11110000111100001111000011110000b       ; reds
            ror eax,16+1

            add eax,ebx                                     ; EAX = red+blue
            add ecx,edx                                     ; ECX = green(lo)+green(hi)

            or ecx,10000000100000001000000010000000b

            mov [edi+80],eax
            mov [edi],ecx

            add esi,4*4*2
            add edi,4

            dec ebp
            jnz .L2

        pop edx
        add edi,80

        dec edx
        jnz .L1

.L3 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RGBRGB:

    push ebp

.L1     mov ebp,20
        push edx

.L2         ; 1st pixel [0r,0g,1b]

            mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            mov bl,[esi+320*2]                              ; EBX = blues
            mov bh,[esi+8+320*2]
            shl ebx,16
            mov bl,[esi+8+8+320*2]
            mov bh,[esi+8+8+8+320*2]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi],eax
            mov [edi+80],ecx
            mov [edi+160],ebx

            ; 2nd pixel [1r,2g,2b]

            mov al,[esi+1+320*2]                            ; EAX = reds
            mov ah,[esi+8+1+320*2]
            shl eax,16
            mov al,[esi+8+8+1+320*2]
            mov ah,[esi+8+8+8+1+320*2]
            and eax,11111000111110001111100011111000b
            ror eax,16+2

            mov cl,[esi+640*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+640*2]
            shl ecx,16
            mov cl,[esi+8+8+640*2]
            mov ch,[esi+8+8+8+640*2]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1+640*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+640*2]
            shl edx,16
            mov dl,[esi+8+8+1+640*2]
            mov dh,[esi+8+8+8+1+640*2]
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+240],eax
            mov [edi+240+80],ecx
            mov [edi+240+160],ebx

            add edi,4
            add esi,4*4*2

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt16_FAKEMODE3_GRBGRB:

    push ebp

.L1     mov ebp,20
        push edx

.L2         ; 1st pixel [0g,0r,1b]

            mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            mov bl,[esi+320*2]                              ; EBX = blues
            mov bh,[esi+8+320*2]
            shl ebx,16
            mov bl,[esi+8+8+320*2]
            mov bh,[esi+8+8+8+320*2]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi],ecx        ; G0
            mov [edi+80],eax     ; R0    
            mov [edi+160],ebx    ; B1

            ; 2nd pixel [1g,2r,2b]

            mov al,[esi+1+640*2]                            ; EAX = reds
            mov ah,[esi+8+1+640*2]
            shl eax,16
            mov al,[esi+8+8+1+640*2]
            mov ah,[esi+8+8+8+1+640*2]
            and eax,11111000111110001111100011111000b
            ror eax,16+2

            mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+320*2]
            shl ecx,16
            mov cl,[esi+8+8+320*2]
            mov ch,[esi+8+8+8+320*2]
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+320*2]
            shl edx,16
            mov dl,[esi+8+8+1+320*2]
            mov dh,[esi+8+8+8+1+320*2]
            and edx,00000111000001110000011100000111b       
            ror edx,16-3

            mov bl,[esi+640*2]                              ; EBX = blues
            mov bh,[esi+8+640*2]
            shl ebx,16
            mov bl,[esi+8+8+640*2]
            mov bh,[esi+8+8+8+640*2]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+240],ecx      ; G1
            mov [edi+240+80],eax   ; R2   
            mov [edi+240+160],ebx  ; B2

            add edi,4
            add esi,4*4*2

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RBGRBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         ; 1st pixel [0r,0b,1g]

            mov bl,[esi]                                    ; EBX = blues
            mov bh,[esi+8]
            shl ebx,16
            mov bl,[esi+8+8]
            mov bh,[esi+8+8+8]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            mov al,[esi+1]                                  ; EAX = reds
            mov ah,[esi+8+1]
            shl eax,16
            mov al,[esi+8+8+1]
            mov ah,[esi+8+8+8+1]
            and eax,11111000111110001111100011111000b       
            ror eax,16+2

            mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+320*2]
            shl ecx,16
            mov cl,[esi+8+8+320*2]
            mov ch,[esi+8+8+8+320*2]
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov [edi],eax       ; R0
            
            mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+320*2]
            shl edx,16
            mov dl,[esi+8+8+1+320*2]
            mov dh,[esi+8+8+8+1+320*2]
            mov eax,edx                                     ; setup eax = r1
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+80],ebx    ; B0
            mov [edi+160],ecx   ; G1

            ; 2nd pixel [1r,2b,2g]

            and eax,11111000111110001111100011111000b
            ror eax,16+2

            mov cl,[esi+640*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+640*2]
            shl ecx,16
            mov cl,[esi+8+8+640*2]
            mov ch,[esi+8+8+8+640*2]
            mov ebx,ecx                                     ; setup blues in ebx
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1+640*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+640*2]
            shl edx,16
            mov dl,[esi+8+8+1+640*2]
            mov dh,[esi+8+8+8+1+640*2]
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+240],eax      ; R1
            mov [edi+240+80],ebx   ; B2
            mov [edi+240+160],ecx  ; G2

            add edi,4
            add esi,4*4*2

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt16_FAKEMODE3_GRBRBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         ; 1st pixel [0g,0r,1b]

            mov cl,[esi]                                    ; ECX = greens (low 3 bits)
            mov ch,[esi+8]
            shl ecx,16
            mov cl,[esi+8+8]
            mov ch,[esi+8+8+8]
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1]
            shl edx,16
            mov dl,[esi+8+8+1]
            mov dh,[esi+8+8+8+1]
            mov eax,edx                                     ; setup reds in eax
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            mov bl,[esi+320*2]                              ; EBX = blues
            mov bh,[esi+8+320*2]
            shl ebx,16
            mov bl,[esi+8+8+320*2]
            mov bh,[esi+8+8+8+320*2]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            and eax,11111000111110001111100011111000b       ; EAX = reds
            ror eax,16+2

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi],ecx        ; G0
            mov [edi+80],eax     ; R0    
            mov [edi+160],ebx    ; B1

            ; 2nd pixel [1r,2b,2g]

            mov al,[esi+1+320*2]                            ; EAX = reds
            mov ah,[esi+8+1+320*2]
            shl eax,16
            mov al,[esi+8+8+1+320*2]
            mov ah,[esi+8+8+8+1+320*2]
            and eax,11111000111110001111100011111000b
            ror eax,16+2

            mov cl,[esi+640*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+640*2]
            shl ecx,16
            mov cl,[esi+8+8+640*2]
            mov ch,[esi+8+8+8+640*2]
            mov ebx,ecx                                     ; setup ebx = blues
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov dl,[esi+1+640*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+640*2]
            shl edx,16
            mov dl,[esi+8+8+1+640*2]
            mov dh,[esi+8+8+8+1+640*2]
            and edx,00000111000001110000011100000111b       
            ror edx,16-3
   
            and ebx,00011111000111110001111100011111b       ; EBX = blues
            ror ebx,15
            add ebx,80808080h

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+240],eax       ; R1   
            mov [edi+240+80],ebx    ; B2
            mov [edi+240+160],ecx   ; G2

            add edi,4
            add esi,4*4*2

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RBGGRB:

    push ebp

.L1     mov ebp,20
        push edx

.L2         ; 1st pixel [0r,0b,1g]

            mov bl,[esi]                                    ; EBX = blues
            mov bh,[esi+8]
            shl ebx,16
            mov bl,[esi+8+8]
            mov bh,[esi+8+8+8]
            and ebx,00011111000111110001111100011111b       
            ror ebx,15
            add ebx,80808080h

            mov al,[esi+1]                                  ; EAX = reds
            mov ah,[esi+8+1]
            shl eax,16
            mov al,[esi+8+8+1]
            mov ah,[esi+8+8+8+1]
            and eax,11111000111110001111100011111000b       
            ror eax,16+2

            mov [edi+80],ebx    ; B0

            mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
            mov dh,[esi+8+1+320*2]
            shl edx,16
            mov dl,[esi+8+8+1+320*2]
            mov dh,[esi+8+8+8+1+320*2]
            mov ebx,edx                                     ; setup ebx = g1 (hi)
            and edx,00000111000001110000011100000111b
            ror edx,16-3

            mov [edi],eax       ; R0

            mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
            mov ch,[esi+8+320*2]
            shl ecx,16
            mov cl,[esi+8+8+320*2]
            mov ch,[esi+8+8+8+320*2]
            mov eax,ecx                                     ; setup eax = g1 (lo)
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5
   
            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+160],ecx   ; G1

            ; 2nd pixel [1g,2r,2b]

            mov ecx,eax                                     ; ECX = greens (low 3 bits)
            and ecx,11100000111000001110000011100000b
            ror ecx,16+5

            mov edx,ebx                                     ; EDX = greens (hi 3 bits)
            and edx,00000111000001110000011100000111b
            ror edx,16-3
   
            mov al,[esi+1+640*2]                            ; EAX = reds
            mov ah,[esi+8+1+640*2]
            shl eax,16
            mov al,[esi+8+8+1+640*2]
            mov ah,[esi+8+8+8+1+640*2]
            and eax,11111000111110001111100011111000b       
            ror eax,16+2

            mov bl,[esi+640*2]                              ; EBX = blues
            mov bh,[esi+8+640*2]
            shl ebx,16
            mov bl,[esi+8+8+640*2]
            mov bh,[esi+8+8+8+640*2]
            and ebx,00011111000111110001111100011111b
            ror ebx,15
            add ebx,80808080h

            add ecx,edx                                     ; ECX = green(lo)+green(hi)
            add ecx,40404040h

            mov [edi+240],ecx       ; G1
            mov [edi+240+80],eax    ; R2
            mov [edi+240+160],ebx   ; B2

            add edi,4
            add esi,4*4*2

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RGBR:

    push ebp
    mov ebp,20

.L1     mov cl,[esi]                                    ; ECX = greens (low 3 bits)
        mov ch,[esi+8]
        shl ecx,16
        mov cl,[esi+8+8]
        mov ch,[esi+8+8+8]
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5

        mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1]
        shl edx,16
        mov dl,[esi+8+8+1]
        mov dh,[esi+8+8+8+1]
        mov eax,edx                                     ; setup reds in eax
        and edx,00000111000001110000011100000111b
        ror edx,16-3
   
        mov bl,[esi+320*2]                              ; EBX = blues
        mov bh,[esi+8+320*2]
        shl ebx,16
        mov bl,[esi+8+8+320*2]
        mov bh,[esi+8+8+8+320*2]
        and ebx,00011111000111110001111100011111b       
        ror ebx,15
        add ebx,80808080h

        and eax,11111000111110001111100011111000b       ; EAX = reds
        ror eax,16+2

        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi],eax
        mov [edi+80],ecx
        mov [edi+160],ebx

        mov al,[esi+1+320*2]                            ; EAX = reds
        mov ah,[esi+8+1+320*2]
        shl eax,16
        mov al,[esi+8+8+1+320*2]
        mov ah,[esi+8+8+8+1+320*2]
        and eax,11111000111110001111100011111000b
        ror eax,16+2

        mov [edi+240],eax

        add edi,4
        add esi,4*4*2

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt16_FAKEMODE3_GRBG:

    push ebp
    mov ebp,20

.L1     mov cl,[esi]                                    ; ECX = greens (low 3 bits)
        mov ch,[esi+8]
        shl ecx,16
        mov cl,[esi+8+8]
        mov ch,[esi+8+8+8]
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5

        mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1]
        shl edx,16
        mov dl,[esi+8+8+1]
        mov dh,[esi+8+8+8+1]
        mov eax,edx                                     ; setup reds in eax
        and edx,00000111000001110000011100000111b
        ror edx,16-3
   
        mov bl,[esi+320*2]                              ; EBX = blues
        mov bh,[esi+8+320*2]
        shl ebx,16
        mov bl,[esi+8+8+320*2]
        mov bh,[esi+8+8+8+320*2]
        and ebx,00011111000111110001111100011111b       
        ror ebx,15
        add ebx,80808080h

        and eax,11111000111110001111100011111000b       ; EAX = reds
        ror eax,16+2

        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi],ecx        ; G0
        mov [edi+80],eax     ; R0    
        mov [edi+160],ebx    ; B1

        mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
        mov ch,[esi+8+320*2]
        shl ecx,16
        mov cl,[esi+8+8+320*2]
        mov ch,[esi+8+8+8+320*2]
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5
                                                        
        mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1+320*2]
        shl edx,16
        mov dl,[esi+8+8+1+320*2]
        mov dh,[esi+8+8+8+1+320*2]
        and edx,00000111000001110000011100000111b       
        ror edx,16-3
   
        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi+240],ecx      ; G1

        add edi,4
        add esi,4*4*2

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RBGR:

    push ebp
    mov ebp,20

.L1     mov bl,[esi]                                    ; EBX = blues
        mov bh,[esi+8]
        shl ebx,16
        mov bl,[esi+8+8]
        mov bh,[esi+8+8+8]
        and ebx,00011111000111110001111100011111b       
        ror ebx,15
        add ebx,80808080h

        mov al,[esi+1]                                  ; EAX = reds
        mov ah,[esi+8+1]
        shl eax,16
        mov al,[esi+8+8+1]
        mov ah,[esi+8+8+8+1]
        and eax,11111000111110001111100011111000b       
        ror eax,16+2
    
        mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
        mov ch,[esi+8+320*2]
        shl ecx,16
        mov cl,[esi+8+8+320*2]
        mov ch,[esi+8+8+8+320*2]
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5

        mov [edi],eax       ; R0

        mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1+320*2]
        shl edx,16
        mov dl,[esi+8+8+1+320*2]
        mov dh,[esi+8+8+8+1+320*2]
        mov eax,edx                                     ; setup eax = r1
        and edx,00000111000001110000011100000111b
        ror edx,16-3
   
        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi+80],ebx    ; B0
        mov [edi+160],ecx   ; G1

        and eax,11111000111110001111100011111000b
        ror eax,16+2

        mov [edi+240],eax      ; R1

        add edi,4
        add esi,4*4*2

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt16_FAKEMODE3_GRBR:

    push ebp
    mov ebp,20

.L1     mov cl,[esi]                                    ; ECX = greens (low 3 bits)
        mov ch,[esi+8]
        shl ecx,16
        mov cl,[esi+8+8]
        mov ch,[esi+8+8+8]
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5

        mov dl,[esi+1]                                  ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1]
        shl edx,16
        mov dl,[esi+8+8+1]
        mov dh,[esi+8+8+8+1]
        mov eax,edx                                     ; setup reds in eax
        and edx,00000111000001110000011100000111b
        ror edx,16-3
   
        mov bl,[esi+320*2]                              ; EBX = blues
        mov bh,[esi+8+320*2]
        shl ebx,16
        mov bl,[esi+8+8+320*2]
        mov bh,[esi+8+8+8+320*2]
        and ebx,00011111000111110001111100011111b       
        ror ebx,15
        add ebx,80808080h

        and eax,11111000111110001111100011111000b       ; EAX = reds
        ror eax,16+2

        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi],ecx        ; G0
        mov [edi+80],eax     ; R0    
        mov [edi+160],ebx    ; B1

        mov al,[esi+1+320*2]                            ; EAX = reds
        mov ah,[esi+8+1+320*2]
        shl eax,16
        mov al,[esi+8+8+1+320*2]
        mov ah,[esi+8+8+8+1+320*2]
        and eax,11111000111110001111100011111000b
        ror eax,16+2

        mov [edi+240],eax       ; R1   

        add edi,4
        add esi,4*4*2

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt16_FAKEMODE3_RBGG:

    push ebp
    mov ebp,20

.L1     mov bl,[esi]                                    ; EBX = blues
        mov bh,[esi+8]
        shl ebx,16
        mov bl,[esi+8+8]
        mov bh,[esi+8+8+8]
        and ebx,00011111000111110001111100011111b       
        ror ebx,15
        add ebx,80808080h

        mov al,[esi+1]                                  ; EAX = reds
        mov ah,[esi+8+1]
        shl eax,16
        mov al,[esi+8+8+1]
        mov ah,[esi+8+8+8+1]
        and eax,11111000111110001111100011111000b       
        ror eax,16+2

        mov [edi+80],ebx    ; B0
   
        mov dl,[esi+1+320*2]                            ; EDX = greens (high 3 bits)
        mov dh,[esi+8+1+320*2]
        shl edx,16
        mov dl,[esi+8+8+1+320*2]
        mov dh,[esi+8+8+8+1+320*2]
        mov ebx,edx                                     ; setup ebx = g1 (hi)
        and edx,00000111000001110000011100000111b
        ror edx,16-3

        mov [edi],eax       ; R0

        mov cl,[esi+320*2]                              ; ECX = greens (low 3 bits)
        mov ch,[esi+8+320*2]
        shl ecx,16
        mov cl,[esi+8+8+320*2]
        mov ch,[esi+8+8+8+320*2]
        mov eax,ecx                                     ; setup eax = g1 (lo)
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5
   
        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi+160],ecx   ; G1

        mov ecx,eax                                     ; ECX = greens (low 3 bits)
        and ecx,11100000111000001110000011100000b
        ror ecx,16+5

        mov edx,ebx                                     ; EDX = greens (hi 3 bits)
        and edx,00000111000001110000011100000111b
        ror edx,16-3

        add ecx,edx                                     ; ECX = green(lo)+green(hi)
        add ecx,40404040h

        mov [edi+240],ecx       ; G1

        add edi,4
        add esi,4*4*2

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt8_FAKEMODE1_RGB:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         mov cl,[esi+4+4]                                ; reds
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; greens
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi+80],eax

            mov cl,[esi+4+4]                                ; blues
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]
            mov [edi+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,160

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE1_RBG:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         mov cl,[esi+4+4]                                ; reds
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; greens
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi+160],eax

            mov cl,[esi+4+4]                                ; blues
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]
            mov [edi+80],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,160

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE1_GRB:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         mov cl,[esi+4+4]                                ; reds
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi+80],eax

            mov cl,[esi+4+4]                                ; greens
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; blues
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]
            mov [edi+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,160

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE2_RBG:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi+4+4]                                ; blues
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]

            mov cl,[esi+4+4]                                ; reds
            mov dl,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov dh,[ebx+ecx*4+2]
            shl edx,16
            mov cl,[esi]                                    
            mov dl,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov dh,[ebx+ecx*4+2]
            add edx,eax

            mov cl,[esi+4+4]                                ; greens
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]

            mov [edi],edx
            mov [edi+80],eax

            add esi,4*4
            add edi,4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,80

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret






PlaneBlt8_FAKEMODE2_GBR:

    push ebp

.L1     mov ebp,20
        push edx

.L2         mov cl,[esi+4+4]                                ; blues
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]

            mov cl,[esi+4+4]                                ; reds
            mov dl,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov dh,[ebx+ecx*4+2]
            shl edx,16
            mov cl,[esi]                                    
            mov dl,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov dh,[ebx+ecx*4+2]
            add edx,eax

            mov cl,[esi+4+4]                                ; greens
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]

            mov [edi+80],edx
            mov [edi],eax

            add esi,4*4
            add edi,4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,80

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RGBRGB:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         ; first RGB line [0r,0g,1b]
   
            mov cl,[esi+4+4]                                ; 0r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; 0g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi+80],eax

            mov cl,[esi+4+4+320]                            ; 1b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+320]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4]
            mov [edi+160],eax

            ; second RGB line [1r,2g,2b]
   
            mov cl,[esi+4+4+320]                            ; 1r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+2]
            mov [edi+240],eax

            mov cl,[esi+4+4+640]                            ; 2g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+640]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4+1]
            mov [edi+240+80],eax

            mov cl,[esi+4+4+640]                            ; 2b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+640]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4]
            mov [edi+240+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_GRBGRB:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         ; first RGB line [0g,0r,1b]
   
            mov cl,[esi+4+4]                                ; 0g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; 0r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi+80],eax

            mov cl,[esi+4+4+320]                            ; 1b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+320]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4]
            mov [edi+160],eax

            ; second RGB line [1g,2r,2b]

            mov cl,[esi+4+4+320]                            ; 1g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+1]
            mov [edi+240],eax

            mov cl,[esi+4+4+640]                            ; 2r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi+640]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4+2]
            mov [edi+240+80],eax

            mov cl,[esi+4+4+640]                            ; 2b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+640]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4]
            mov [edi+240+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RBGRBG:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         ; first RGB line [0r,0b,1g]
   
            mov cl,[esi+4+4]                                ; 0r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; 0b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]
            mov [edi+80],eax

            mov cl,[esi+4+4+320]                            ; 1g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+1]
            mov [edi+160],eax

            ; second RGB line [1r,2b,2g]
   
            mov cl,[esi+4+4+320]                            ; 1r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+2]
            mov [edi+240],eax

            mov cl,[esi+4+4+640]                            ; 2b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+640]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4]
            mov [edi+240+80],eax

            mov cl,[esi+4+4+640]                            ; 2g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+640]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4+1]
            mov [edi+240+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_GRBRBG:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         ; first RGB line [0g,0r,1b]
   
            mov cl,[esi+4+4]                                ; 0g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+1]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; 0r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi+80],eax

            mov cl,[esi+4+4+320]                            ; 1b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+320]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4]
            mov [edi+160],eax

            ; second RGB line [1r,2b,2g]
   
            mov cl,[esi+4+4+320]                            ; 1r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+2]
            mov [edi+240],eax

            mov cl,[esi+4+4+640]                            ; 2b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+640]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4]
            mov [edi+240+80],eax

            mov cl,[esi+4+4+640]                            ; 2g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+640]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4+1]
            mov [edi+240+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RBGGRB:

    push ebp
    xor ecx,ecx

.L1     mov ebp,20
        push edx        ; NOTE: no need to push edx!

.L2         ; first RGB line [0r,0b,1g]
   
            mov cl,[esi+4+4]                                ; 0r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4+2]
            mov [edi],eax

            mov cl,[esi+4+4]                                ; 0b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4]
            mov ah,[ebx+ecx*4]
            mov [edi+80],eax

            mov cl,[esi+4+4+320]                            ; 1g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+1]
            mov [edi+160],eax

            ; second RGB line [1g,2r,2b]

            mov cl,[esi+4+4+320]                            ; 1g
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+4+4+320]
            mov ah,[ebx+ecx*4+1]
            shl eax,16
            mov cl,[esi+320]                                    
            mov al,[ebx+ecx*4+1]
            mov cl,[esi+4+320]
            mov ah,[ebx+ecx*4+1]
            mov [edi+240],eax

            mov cl,[esi+4+4+640]                            ; 2r
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4+2]
            shl eax,16
            mov cl,[esi+640]                                    
            mov al,[ebx+ecx*4+2]
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4+2]
            mov [edi+240+80],eax

            mov cl,[esi+4+4+640]                            ; 2b
            mov al,[ebx+ecx*4]
            mov cl,[esi+4+4+4+640]
            mov ah,[ebx+ecx*4]
            shl eax,16
            mov cl,[esi+640]
            mov al,[ebx+ecx*4]                              
            mov cl,[esi+4+640]
            mov ah,[ebx+ecx*4]
            mov [edi+240+160],eax

            add edi,4
            add esi,4*4

            dec ebp
            jz .L3
            jmp .L2

.L3     pop edx
        add edi,320+80
        add esi,320*2

        dec edx
        jz .L4
        jmp .L1

.L4 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RGBR:

    push ebp
    xor ecx,ecx
    mov ebp,20

.L1     mov cl,[esi+4+4]                                ; 0r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+2]
        mov [edi],eax

        mov cl,[esi+4+4]                                ; 0g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+1]
        mov [edi+80],eax

        mov cl,[esi+4+4+320]                            ; 1b
        mov al,[ebx+ecx*4]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4]
        shl eax,16
        mov cl,[esi+320]
        mov al,[ebx+ecx*4]                              
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4]
        mov [edi+160],eax

        mov cl,[esi+4+4+320]                            ; 1r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+2]
        mov [edi+240],eax

        add edi,4
        add esi,4*4

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt8_FAKEMODE3_GRBG:

    push ebp
    xor ecx,ecx
    mov ebp,20

.L1     mov cl,[esi+4+4]                                ; 0g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+1]
        mov [edi],eax

        mov cl,[esi+4+4]                                ; 0r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+2]
        mov [edi+80],eax

        mov cl,[esi+4+4+320]                            ; 1b
        mov al,[ebx+ecx*4]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4]
        shl eax,16
        mov cl,[esi+320]
        mov al,[ebx+ecx*4]                              
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4]
        mov [edi+160],eax
   
        mov cl,[esi+4+4+320]                            ; 1g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+1]
        mov [edi+240],eax
   
        add edi,4
        add esi,4*4

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RBGR:

    push ebp
    xor ecx,ecx
    mov ebp,20

.L1     mov cl,[esi+4+4]                                ; 0r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+2]
        mov [edi],eax

        mov cl,[esi+4+4]                                ; 0b
        mov al,[ebx+ecx*4]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4]
        shl eax,16
        mov cl,[esi]
        mov al,[ebx+ecx*4]                              
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4]
        mov [edi+80],eax
   
        mov cl,[esi+4+4+320]                            ; 1g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+1]
        mov [edi+160],eax

        mov cl,[esi+4+4+320]                            ; 1r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+2]
        mov [edi+240],eax
    
        add edi,4
        add esi,4*4

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt8_FAKEMODE3_GRBR:

    push ebp
    xor ecx,ecx
    mov ebp,20

.L1     mov cl,[esi+4+4]                                ; 0g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+1]
        mov [edi],eax

        mov cl,[esi+4+4]                                ; 0r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+2]
        mov [edi+80],eax

        mov cl,[esi+4+4+320]                            ; 1b
        mov al,[ebx+ecx*4]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4]
        shl eax,16
        mov cl,[esi+320]
        mov al,[ebx+ecx*4]                              
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4]
        mov [edi+160],eax

        mov cl,[esi+4+4+320]                            ; 1r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+2]
        mov [edi+240],eax

        add edi,4
        add esi,4*4

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret







PlaneBlt8_FAKEMODE3_RBGG:

    push ebp
    xor ecx,ecx
    mov ebp,20

.L1     mov cl,[esi+4+4]                                ; 0r
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4+2]
        shl eax,16
        mov cl,[esi]                                    
        mov al,[ebx+ecx*4+2]
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4+2]
        mov [edi],eax
    
        mov cl,[esi+4+4]                                ; 0b
        mov al,[ebx+ecx*4]
        mov cl,[esi+4+4+4]
        mov ah,[ebx+ecx*4]
        shl eax,16
        mov cl,[esi]
        mov al,[ebx+ecx*4]                              
        mov cl,[esi+4]
        mov ah,[ebx+ecx*4]
        mov [edi+80],eax

        mov cl,[esi+4+4+320]                            ; 1g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+1]
        mov [edi+160],eax

        mov cl,[esi+4+4+320]                            ; 1g
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+4+4+320]
        mov ah,[ebx+ecx*4+1]
        shl eax,16
        mov cl,[esi+320]                                    
        mov al,[ebx+ecx*4+1]
        mov cl,[esi+4+320]
        mov ah,[ebx+ecx*4+1]
        mov [edi+240],eax

        add edi,4
        add esi,4*4

        dec ebp
        jz .L2
        jmp .L1

.L2 pop ebp
    ret





SECTION .data

DestOffset   DD 0
SourceOffset DD 0
