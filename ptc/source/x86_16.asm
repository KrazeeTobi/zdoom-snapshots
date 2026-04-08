;----------------------------------------------------------------------------------------------------------------------------------------
; 16bit -> X pixel format conversions routines for PTC (intel x86)
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

GLOBAL _Convert16_ARGB8888_X86
GLOBAL _Convert16_ABGR8888_X86
GLOBAL _Convert16_RGBA8888_X86
GLOBAL _Convert16_BGRA8888_X86
GLOBAL _Convert16_RGB888_X86
GLOBAL _Convert16_BGR888_X86
GLOBAL _Convert16_BGR565_X86
GLOBAL _Convert16_ARGB1555_X86
GLOBAL _Convert16_ABGR1555_X86
GLOBAL _Convert16_GREY8_X86
GLOBAL _Convert16_RGB332_X86
GLOBAL _AreaConvert16_ARGB8888_X86
GLOBAL _AreaConvert16_ABGR8888_X86
GLOBAL _AreaConvert16_RGBA8888_X86
GLOBAL _AreaConvert16_BGRA8888_X86
GLOBAL _AreaConvert16_RGB888_X86
GLOBAL _AreaConvert16_BGR888_X86
GLOBAL _AreaConvert16_BGR565_X86
GLOBAL _AreaConvert16_ARGB1555_X86
GLOBAL _AreaConvert16_ABGR1555_X86
GLOBAL _AreaConvert16_GREY8_X86
GLOBAL _AreaConvert16_RGB332_X86

EXTERN _Convert16_ARGB8888_LUT_X86
EXTERN _Convert16_ABGR8888_LUT_X86
EXTERN _Convert16_RGBA8888_LUT_X86
EXTERN _Convert16_BGRA8888_LUT_X86

SECTION .text









_Convert16_ARGB8888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor ebx,ebx
.L1 mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_ARGB8888_LUT_X86+ebx*8]     ; eax = ARGB8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_ARGB8888_LUT_X86+ebx*8+4]   ; edx = ARGB8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll twice
    mov ebp,ecx
    shr ebp,1
    
    ; point arrays to end
    lea esi,[esi+ebp*4]
    lea edi,[edi+ebp*8]

    ; negative counter 
    neg ebp

    ; clear
    xor ebx,ebx
    xor ecx,ecx

    ; prestep
    mov cl,[esi+ebp*4+0]
    mov bl,[esi+ebp*4+1]

.L4     mov edx,[_Convert16_ARGB8888_LUT_X86+ecx*8]
        mov cl,[esi+ebp*4+2]

        mov eax,[_Convert16_ARGB8888_LUT_X86+ebx*8+4]
        mov bl,[esi+ebp*4+3]

        add eax,edx
        mov edx,[_Convert16_ARGB8888_LUT_X86+ecx*8]

        mov [edi+ebp*8],eax
        mov eax,[_Convert16_ARGB8888_LUT_X86+ebx*8+4]

        add eax,edx
        mov cl,[esi+ebp*4+4]

        mov [edi+ebp*8+4],eax
        mov bl,[esi+ebp*4+5]

        inc ebp
        jnz .L4

    ; tail
    pop ecx
    and ecx,1
    jz .L6
    xor ebx,ebx
    mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_ARGB8888_LUT_X86+ebx*8]     ; eax = ARGB8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_ARGB8888_LUT_X86+ebx*8+4]   ; edx = ARGB8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4

.L6 pop ebp
    ret






_Convert16_ABGR8888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor ebx,ebx
.L1 mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8]     ; eax = ABGR8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_ABGR8888_LUT_X86+ebx*8+4]   ; edx = ABGR8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll twice
    mov ebp,ecx
    shr ebp,1
    
    ; point arrays to end
    lea esi,[esi+ebp*4]
    lea edi,[edi+ebp*8]

    ; negative counter 
    neg ebp

    ; clear
    xor ebx,ebx
    xor ecx,ecx

    ; prestep
    mov cl,[esi+ebp*4+0]
    mov bl,[esi+ebp*4+1]

.L4     mov edx,[_Convert16_ABGR8888_LUT_X86+ecx*8]
        mov cl,[esi+ebp*4+2]

        mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8+4]
        mov bl,[esi+ebp*4+3]

        add eax,edx
        mov edx,[_Convert16_ABGR8888_LUT_X86+ecx*8]

        mov [edi+ebp*8],eax
        mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8+4]

        add eax,edx
        mov cl,[esi+ebp*4+4]

        mov [edi+ebp*8+4],eax
        mov bl,[esi+ebp*4+5]

        inc ebp
        jnz .L4

    ; tail
    pop ecx
    and ecx,1
    jz .L6
    xor ebx,ebx
    mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8]     ; eax = ABGR8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_ABGR8888_LUT_X86+ebx*8+4]   ; edx = ABGR8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4

.L6 pop ebp
    ret






_Convert16_RGBA8888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor ebx,ebx
.L1 mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_RGBA8888_LUT_X86+ebx*8]     ; eax = RGBA8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_RGBA8888_LUT_X86+ebx*8+4]   ; edx = RGBA8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll twice
    mov ebp,ecx
    shr ebp,1
    
    ; point arrays to end
    lea esi,[esi+ebp*4]
    lea edi,[edi+ebp*8]

    ; negative counter 
    neg ebp

    ; clear
    xor ebx,ebx
    xor ecx,ecx

    ; prestep
    mov cl,[esi+ebp*4+0]
    mov bl,[esi+ebp*4+1]

.L4     mov edx,[_Convert16_RGBA8888_LUT_X86+ecx*8]
        mov cl,[esi+ebp*4+2]

        mov eax,[_Convert16_RGBA8888_LUT_X86+ebx*8+4]
        mov bl,[esi+ebp*4+3]

        add eax,edx
        mov edx,[_Convert16_RGBA8888_LUT_X86+ecx*8]

        mov [edi+ebp*8],eax
        mov eax,[_Convert16_RGBA8888_LUT_X86+ebx*8+4]

        add eax,edx
        mov cl,[esi+ebp*4+4]

        mov [edi+ebp*8+4],eax
        mov bl,[esi+ebp*4+5]

        inc ebp
        jnz .L4

    ; tail
    pop ecx
    and ecx,1
    jz .L6
    xor ebx,ebx
    mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_RGBA8888_LUT_X86+ebx*8]     ; eax = RGBA8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_RGBA8888_LUT_X86+ebx*8+4]   ; edx = RGBA8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4

.L6 pop ebp
    ret






_Convert16_BGRA8888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor ebx,ebx
.L1 mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_BGRA8888_LUT_X86+ebx*8]     ; eax = BGRA8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_BGRA8888_LUT_X86+ebx*8+4]   ; edx = BGRA8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll twice
    mov ebp,ecx
    shr ebp,1
    
    ; point arrays to end
    lea esi,[esi+ebp*4]
    lea edi,[edi+ebp*8]

    ; negative counter 
    neg ebp

    ; clear
    xor ebx,ebx
    xor ecx,ecx

    ; prestep
    mov cl,[esi+ebp*4+0]
    mov bl,[esi+ebp*4+1]

.L4     mov edx,[_Convert16_BGRA8888_LUT_X86+ecx*8]
        mov cl,[esi+ebp*4+2]

        mov eax,[_Convert16_BGRA8888_LUT_X86+ebx*8+4]
        mov bl,[esi+ebp*4+3]

        add eax,edx
        mov edx,[_Convert16_BGRA8888_LUT_X86+ecx*8]

        mov [edi+ebp*8],eax
        mov eax,[_Convert16_BGRA8888_LUT_X86+ebx*8+4]

        add eax,edx
        mov cl,[esi+ebp*4+4]

        mov [edi+ebp*8+4],eax
        mov bl,[esi+ebp*4+5]

        inc ebp
        jnz .L4

    ; tail
    pop ecx
    and ecx,1
    jz .L6
    xor ebx,ebx
    mov bl,[esi]                                    ; ebx = lower byte pixel 1
    mov eax,[_Convert16_BGRA8888_LUT_X86+ebx*8]     ; eax = BGRA8888 of lower byte pixel 1
    mov bl,[esi+1]                                  ; ebx = upper byte pixel 1
    mov edx,[_Convert16_BGRA8888_LUT_X86+ebx*8+4]   ; edx = BGRA8888 of upper byte pixel 1
    add eax,edx
    mov [edi],eax
    add esi,2
    add edi,4

.L6 pop ebp
    ret






_Convert16_RGB888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor edx,edx
.L1 mov dl,[esi]
    mov eax,[_Convert16_ARGB8888_LUT_X86+edx*8]       ; eax = ARGB8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ARGB8888_LUT_X86+edx*8+4]     ; ebx = ARGB8888 of upper byte
    add eax,ebx                                       ; eax = ARGB8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jnz .L1
.L2 ret


.L3 ; clear edx
    xor edx,edx

.L4 ; head
    mov eax,edi
    and eax,11b
    jz .L5
    mov dl,[esi]
    mov eax,[_Convert16_ARGB8888_LUT_X86+edx*8]       ; eax = ARGB8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ARGB8888_LUT_X86+edx*8+4]     ; ebx = ARGB8888 of upper byte
    add eax,ebx                                       ; eax = ARGB8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jmp .L4

.L5 ; unroll 4 times
    push ebp
    mov ebp,ecx
    shr ebp,2

    ; clear ebx
    xor ebx,ebx

    ; save count
    push ecx

    ; prestep
    mov bl,[esi+0]                                      ; ebx = lower byte pixel 1
    mov dl,[esi+1]                                      ; edx = upper byte pixel 1

.L6     mov eax,[_Convert16_ARGB8888_LUT_X86+ebx*8]         ; eax = ARGB8888 of lower byte pixel 1
        mov bl,[esi+2]                                      ; ebx = lower byte pixel 2

        mov ecx,[_Convert16_ARGB8888_LUT_X86+edx*8+4]       ; ecx = ARGB8888 of upper byte pixel 1
        mov dl,[esi+3]                                      ; edx = upper byte pixel 2
        
        push ebp                                            ; save ebp
        add eax,ecx                                         ; eax = ARGB8888 of pixel 1

        mov ebp,[_Convert16_ARGB8888_LUT_X86+ebx*8]         ; ebp = ARGB8888 of lower byte pixel 2
        mov ecx,[_Convert16_ARGB8888_LUT_X86+edx*8+4]       ; ecx = ARGB8888 of upper byte pixel 2

        mov bl,[esi+4]                                      ; ebx = lower byte pixel 3
        add ecx,ebp                                         ; ecx = ARGB8888 of pixel 2

        shl ebp,24                                          ; ebp = [b][0][0][0] of pixel 2
        mov dl,[esi+5]                                      ; edx = upper byte pixel 3

        shr ecx,8                                           ; ecx = [0][0][r][g] pixel 2
        add eax,ebp                                         ; eax = [b2][r1][g1][b1] (done)

        mov [edi],eax                                       ; store dword 1
        mov eax,[_Convert16_ARGB8888_LUT_X86+edx*8+4]       ; eax = ARGB8888 of upper byte pixel 3

        mov ebp,[_Convert16_ARGB8888_LUT_X86+ebx*8]         ; ebp = ARGB8888 of lower byte pixel 3
        mov bl,[esi+6]                                      ; ebx = lower byte pixel 4

        add ebp,eax                                         ; ebp = ARGB8888 of pixel 3
        mov dl,[esi+7]                                      ; edx = upper byte pixel 4

        shl ebp,16                                          ; ebp = [g][b][0][0] pixel 3

        shr eax,16                                          ;  al = red component of pixel 3
        add ebp,ecx                                         ; ebp = [g3][b3][r2][g2] (done)

        mov [edi+4],ebp                                     ; store dword 2
        mov ecx,[_Convert16_ARGB8888_LUT_X86+ebx*8]         ; ebx = ARGB8888 of lower byte pixel 4

        mov ebp,[_Convert16_ARGB8888_LUT_X86+edx*8+4]       ; ebp = ARGB8888 of upper byte pixel 4
        mov bl,[esi+4*2+0]                                  ; ebx = lower byte pixel 1

        add ecx,ebp                                         ; ecx = ARGB8888 of pixel 4
        mov dl,[esi+4*2+1]                                  ; edx = upper byte pixel 1

        shl ecx,8                                           ; ecx = [r][g][b][0]
        pop ebp                                             ; restore ebp

        mov cl,al                                           ; ecx = [r4][g4][b4][r3] (done)
        add esi,4*2                                       

        mov [edi+8],ecx                                     ; store dword 3
        add edi,3*4

        dec ebp
        jz .L7

        jmp .L6                                           

.L7 ; check tail
    pop ecx
    and ecx,11b
    jz .L9

.L8 ; tail
    mov dl,[esi]
    mov eax,[_Convert16_ARGB8888_LUT_X86+edx*8]       ; eax = ARGB8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ARGB8888_LUT_X86+edx*8+4]     ; ebx = ARGB8888 of upper byte
    add eax,ebx                                       ; eax = ARGB8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jnz .L8

.L9 pop ebp
    ret





_Convert16_BGR888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
    xor edx,edx
.L1 mov dl,[esi]
    mov eax,[_Convert16_ABGR8888_LUT_X86+edx*8]       ; eax = ABGR8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ABGR8888_LUT_X86+edx*8+4]     ; ebx = ABGR8888 of upper byte
    add eax,ebx                                       ; eax = ABGR8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jnz .L1
.L2 ret


.L3 ; clear edx
    xor edx,edx

.L4 ; head
    mov eax,edi
    and eax,11b
    jz .L5
    mov dl,[esi]
    mov eax,[_Convert16_ABGR8888_LUT_X86+edx*8]       ; eax = ABGR8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ABGR8888_LUT_X86+edx*8+4]     ; ebx = ABGR8888 of upper byte
    add eax,ebx                                       ; eax = ABGR8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jmp .L4

.L5 ; unroll 4 times
    push ebp
    mov ebp,ecx
    shr ebp,2

    ; clear ebx
    xor ebx,ebx

    ; save count
    push ecx

    ; prestep
    mov bl,[esi+0]                                      ; ebx = lower byte pixel 1
    mov dl,[esi+1]                                      ; edx = upper byte pixel 1

.L6     mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8]         ; eax = ABGR8888 of lower byte pixel 1
        mov bl,[esi+2]                                      ; ebx = lower byte pixel 2

        mov ecx,[_Convert16_ABGR8888_LUT_X86+edx*8+4]       ; ecx = ABGR8888 of upper byte pixel 1
        mov dl,[esi+3]                                      ; edx = upper byte pixel 2
        
        push ebp                                            ; save ebp
        add eax,ecx                                         ; eax = ABGR8888 of pixel 1

        mov ecx,[_Convert16_ABGR8888_LUT_X86+ebx*8]         ; ecx = ABGR8888 of lower byte pixel 2
        mov ebp,[_Convert16_ABGR8888_LUT_X86+edx*8+4]       ; ebp = ABGR8888 of upper byte pixel 2

        mov bl,[esi+4]                                      ; ebx = lower byte pixel 3
        add ecx,ebp                                         ; ecx = ABGR8888 of pixel 2

        shl ebp,24                                          ; ebp = [r][0][0][0] of pixel 2
        mov dl,[esi+5]                                      ; edx = upper byte pixel 3

        shr ecx,8                                           ; ecx = [0][0][b][g] pixel 2
        add eax,ebp                                         ; eax = [r2][b1][g1][r1] (done)

        mov [edi],eax                                       ; store dword 1
        mov ebp,[_Convert16_ABGR8888_LUT_X86+edx*8+4]       ; ebp = ABGR8888 of upper byte pixel 3

        mov eax,[_Convert16_ABGR8888_LUT_X86+ebx*8]         ; eax = ABGR8888 of lower byte pixel 3
        mov bl,[esi+6]                                      ; ebx = lower byte pixel 4

        add ebp,eax                                         ; ebp = ABGR8888 of pixel 3
        mov dl,[esi+7]                                      ; edx = upper byte pixel 4

        shl ebp,16                                          ; ebp = [g][r][0][0] pixel 3

        shr eax,16                                          ;  al = blue component of pixel 3
        add ebp,ecx                                         ; ebp = [g3][r3][b2][g2] (done)

        mov [edi+4],ebp                                     ; store dword 2
        mov ecx,[_Convert16_ABGR8888_LUT_X86+ebx*8]         ; ebx = ABGR8888 of lower byte pixel 4

        mov ebp,[_Convert16_ABGR8888_LUT_X86+edx*8+4]       ; ebp = ABGR8888 of upper byte pixel 4
        mov bl,[esi+4*2+0]                                  ; ebx = lower byte pixel 1

        add ecx,ebp                                         ; ecx = ABGR8888 of pixel 4
        mov dl,[esi+4*2+1]                                  ; edx = upper byte pixel 1

        shl ecx,8                                           ; ecx = [b][g][r][0]
        pop ebp                                             ; restore ebp

        mov cl,al                                           ; ecx = [b4][g4][r4][b3] (done)
        add esi,4*2                                       

        mov [edi+8],ecx                                     ; store dword 3
        add edi,3*4

        dec ebp
        jz .L7

        jmp .L6                                           

.L7 ; check tail
    pop ecx
    and ecx,11b
    jz .L9

.L8 ; tail
    mov dl,[esi]
    mov eax,[_Convert16_ABGR8888_LUT_X86+edx*8]       ; eax = ABGR8888 of lower byte
    mov dl,[esi+1]
    mov ebx,[_Convert16_ABGR8888_LUT_X86+edx*8+4]     ; ebx = ABGR8888 of upper byte
    add eax,ebx                                       ; eax = ABGR8888 pixel
    mov [edi],al
    mov [edi+1],ah
    shr eax,16
    mov [edi+2],al
    add esi,2
    add edi,3
    dec ecx
    jnz .L8

.L9 pop ebp
    ret






_Convert16_BGR565_X86:

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    and ebx,11111100000b
    shl edx,11
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov eax,edi
    and eax,11b
    jz .L4
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    and ebx,11111100000b
    shl edx,11
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*4]
    lea edi,[edi+ecx*4]

    ; negative counter 
    neg ecx
    jmp .L6
                              
.L5     mov [edi+ecx*4-4],eax
.L6     mov eax,[esi+ecx*4]

        mov ebx,[esi+ecx*4]
        and eax,07E007E0h         

        mov edx,[esi+ecx*4]
        and ebx,0F800F800h

        shr ebx,11
        and edx,001F001Fh

        shl edx,11
        add eax,ebx

        add eax,edx                 
        inc ecx

        jnz .L5                 
         
    mov [edi+ecx*4-4],eax

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    and ebx,11111100000b
    shl edx,11
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2

.L7 ret






_Convert16_ARGB1555_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    shr ebx,1
    and ebx,0111111111100000b
    and eax,0000000000011111b
    add eax,ebx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov eax,edi
    and eax,11b
    jz .L4
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    shr ebx,1
    and ebx,0111111111100000b
    and eax,0000000000011111b
    add eax,ebx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx

.L4 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll four times
    shr ecx,2
    
    ; point arrays to end
    lea esi,[esi+ecx*8]
    lea edi,[edi+ecx*8]

    ; negative counter 
    xor ebp,ebp
    sub ebp,ecx

.L5     mov eax,[esi+ebp*8]        ; agi?
        mov ecx,[esi+ebp*8+4]
       
        mov ebx,eax
        mov edx,ecx

        and eax,0FFC0FFC0h
        and ecx,0FFC0FFC0h

        shr eax,1
        and ebx,001F001Fh

        shr ecx,1
        and edx,001F001Fh

        add eax,ebx
        add ecx,edx

        mov [edi+ebp*8],eax
        mov [edi+ebp*8+4],ecx

        inc ebp
        jnz .L5                 

    ; tail
    pop ecx
.L6 and ecx,11b
    jz .L7
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    shr ebx,1
    and ebx,0111111111100000b
    and eax,0000000000011111b
    add eax,ebx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx
    jmp .L6

.L7 pop ebp
    ret






_Convert16_ABGR1555_X86:

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    shr ebx,1
    and ebx,1111100000b
    shl edx,10
    and edx,0111110000000000b
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov eax,edi
    and eax,11b
    jz .L4
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    shr ebx,1
    and ebx,1111100000b
    shl edx,10
    and edx,0111110000000000b
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*4]
    lea edi,[edi+ecx*4]

    ; negative counter 
    neg ecx
    jmp .L6
                              
.L5     mov [edi+ecx*4-4],eax
.L6     mov eax,[esi+ecx*4]

        shr eax,1
        mov ebx,[esi+ecx*4]
        
        and eax,03E003E0h         
        mov edx,[esi+ecx*4]

        and ebx,0F800F800h

        shr ebx,11
        and edx,001F001Fh

        shl edx,10
        add eax,ebx

        add eax,edx                 
        inc ecx

        jnz .L5                 
         
    mov [edi+ecx*4-4],eax

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov al,[esi]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    shr eax,11
    and eax,11111b
    shr ebx,1
    and ebx,1111100000b
    shl edx,10
    and edx,0111110000000000b
    add eax,ebx
    add eax,edx
    mov [edi],al
    mov [edi+1],ah
    add esi,2
    add edi,2

.L7 ret







_Convert16_GREY8_X86:

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11111b              ; blue
    shl eax,1
    and ebx,11111100000b        ; green
    shr ebx,4
    and edx,1111100000000000b   ; red
    shr edx,10
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jnz .L1
.L2 ret

.L3 mov eax,edi
    and eax,11b
    jz .L4
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11111b              ; blue
    shl eax,1
    and ebx,11111100000b        ; green
    shr ebx,4
    and edx,1111100000000000b   ; red
    shr edx,10
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jmp .L3

.L4 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll 4 times
    mov ebp,ecx
    shr ebp,2

    ; prestep
    mov dl,[esi+0]
    mov bl,[esi+1]
    mov dh,[esi+2]

.L5     shl edx,16
        mov bh,[esi+3]

        shl ebx,16
        mov dl,[esi+4]
        
        mov bl,[esi+5]
        mov dh,[esi+6]

        mov bh,[esi+7]
        mov ecx,edx                                     ; setup ecx (for greens)

        mov eax,ebx
        and ebx,00000111000001110000011100000111b

        ror ebx,16-4
        and edx,00011111000111110001111100011111b

        ror edx,16-1
        and eax,11111000111110001111100011111000b       ; reds

        ror eax,16+2
        and ecx,11100000111000001110000011100000b       ; greens (low 3 bits)

        ror ecx,16+4
        add eax,ebx

        add eax,ecx
        mov bl,[esi+8+1]
        
        add eax,edx
        mov dl,[esi+8+0]
        
        mov [edi],eax
        mov dh,[esi+8+2]

        add edi,4
        add esi,8

        dec ebp
        jnz .L5                 
    
    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6 ; tail
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11111b              ; blue
    shl eax,1
    and ebx,11111100000b        ; green
    shr ebx,4
    and edx,1111100000000000b   ; red
    shr edx,10
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jnz .L6

.L7 pop ebp
    ret






_Convert16_RGB332_X86:

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11000b              ; blue
    shr eax,3
    and ebx,11100000000b        ; green
    shr ebx,6
    and edx,1110000000000000b   ; red
    shr edx,8
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jnz .L1
.L2 ret

.L3 mov eax,edi
    and eax,11b
    jz .L4
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11000b              ; blue
    shr eax,3
    and ebx,11100000000b        ; green
    shr ebx,6
    and edx,1110000000000000b   ; red
    shr edx,8
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jmp .L3

.L4 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll 4 times
    shr ecx,2

    ; prestep
    mov dl,[esi+0]
    mov bl,[esi+1]
    mov dh,[esi+2]
        
.L5     shl edx,16
        mov bh,[esi+3]
        
        shl ebx,16
        mov dl,[esi+4]

        mov dh,[esi+6]
        mov bl,[esi+5]

        and edx,00011000000110000001100000011000b
        mov bh,[esi+7]

        ror edx,16+3
        mov eax,ebx                                     ; setup eax for reds

        and ebx,00000111000001110000011100000111b
        and eax,11100000111000001110000011100000b       ; reds

        ror ebx,16-2
        add esi,8

        ror eax,16
        add edi,4

        add eax,ebx
        mov bl,[esi+1]                                  ; greens

        add eax,edx
        mov dl,[esi+0]                                  ; blues

        mov [edi-4],eax
        mov dh,[esi+2]

        dec ecx
        jnz .L5                 
    
    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6 ; tail
    mov al,[esi+0]
    mov ah,[esi+1]
    mov ebx,eax
    mov edx,eax
    and eax,11000b              ; blue
    shr eax,3
    and ebx,11100000000b        ; green
    shr ebx,6
    and edx,1110000000000000b   ; red
    shr edx,8
    add eax,ebx
    add eax,edx
    mov [edi],al
    add esi,2
    inc edi
    dec ecx
    jnz .L6

.L7 pop ebp
    ret








;----------------------------------------------------------------------------------------------------------------------------------------
; 16bit -> X area pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = pointer to area convert information            
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,ecx,edx,edi                    
;----------------------------------------------------------------------------------------------------------------------------------------


_AreaConvert16_ARGB8888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_ARGB8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_ABGR8888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_ABGR8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_RGBA8888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_RGBA8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_BGRA8888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_BGRA8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_RGB888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_RGB888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_BGR888_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_BGR888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_BGR565_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_BGR565_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_ARGB1555_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_ARGB1555_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_ABGR1555_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_ABGR1555_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_GREY8_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_GREY8_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert16_RGB332_X86:

    ; setup ebp = data
    push ebp
    mov ebp,esi

    ; check height
    cmp dword [ebp+44],0
    je .L2

    ; setup offsets
    mov esi,[ebp+24]
    mov edi,[ebp+28]

.L1     mov ecx,[ebp+40]
        call _Convert16_RGB332_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret