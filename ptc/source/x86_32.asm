;----------------------------------------------------------------------------------------------------------------------------------------
; 32bit -> X pixel format conversions routines for PTC (intel x86)
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

GLOBAL _Convert32_ABGR8888_X86
GLOBAL _Convert32_RGBA8888_X86
GLOBAL _Convert32_BGRA8888_X86
GLOBAL _Convert32_RGB888_X86
GLOBAL _Convert32_BGR888_X86
GLOBAL _Convert32_RGB565_X86
GLOBAL _Convert32_BGR565_X86
GLOBAL _Convert32_ARGB1555_X86
GLOBAL _Convert32_ABGR1555_X86
GLOBAL _Convert32_GREY8_X86
GLOBAL _Convert32_RGB332_X86
GLOBAL _AreaConvert32_ABGR8888_X86
GLOBAL _AreaConvert32_RGBA8888_X86
GLOBAL _AreaConvert32_BGRA8888_X86
GLOBAL _AreaConvert32_RGB888_X86
GLOBAL _AreaConvert32_BGR888_X86
GLOBAL _AreaConvert32_RGB565_X86
GLOBAL _AreaConvert32_BGR565_X86
GLOBAL _AreaConvert32_ARGB1555_X86
GLOBAL _AreaConvert32_ABGR1555_X86
GLOBAL _AreaConvert32_GREY8_X86
GLOBAL _AreaConvert32_RGB332_X86

SECTION .text




        



_Convert32_ABGR8888_X86:                

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov edx,[esi]
    bswap edx
    ror edx,8
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; unroll four times
    mov ebp,ecx
    shr ebp,2
    
    ; save count
    push ecx

.L4     mov eax,[esi]
        mov ebx,[esi+4]

        bswap eax

        bswap ebx

        ror eax,8
        mov ecx,[esi+8]

        ror ebx,8
        mov edx,[esi+12]

        bswap ecx

        bswap edx

        ror ecx,8
        mov [edi+0],eax

        ror edx,8
        mov [edi+4],ebx

        mov [edi+8],ecx
        mov [edi+12],edx

        add esi,16
        add edi,16

        dec ebp
        jnz .L4                 

    ; check tail
    pop ecx
    and ecx,11b
    jz .L6

.L5 ; tail loop
    mov edx,[esi]
    bswap edx
    ror edx,8
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L5

.L6 pop ebp
    ret






_Convert32_RGBA8888_X86:                

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov edx,[esi]
    rol edx,8
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; unroll four times
    mov ebp,ecx
    shr ebp,2
    
    ; save count
    push ecx

.L4     mov eax,[esi]
        mov ebx,[esi+4]

        rol eax,8
        mov ecx,[esi+8]

        rol ebx,8
        mov edx,[esi+12]

        rol ecx,8
        mov [edi+0],eax

        rol edx,8
        mov [edi+4],ebx

        mov [edi+8],ecx
        mov [edi+12],edx

        add esi,16
        add edi,16

        dec ebp
        jnz .L4                 

    ; check tail
    pop ecx
    and ecx,11b
    jz .L6

.L5 ; tail loop
    mov edx,[esi]
    rol edx,8
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L5

.L6 pop ebp
    ret






_Convert32_BGRA8888_X86:                

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov edx,[esi]
    bswap edx
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L1
.L2 ret

.L3 ; save ebp
    push ebp

    ; unroll four times
    mov ebp,ecx
    shr ebp,2
    
    ; save count
    push ecx

.L4     mov eax,[esi]
        mov ebx,[esi+4]

        mov ecx,[esi+8]
        mov edx,[esi+12]

        bswap eax

        bswap ebx

        bswap ecx

        bswap edx

        mov [edi+0],eax
        mov [edi+4],ebx

        mov [edi+8],ecx
        mov [edi+12],edx

        add esi,16
        add edi,16

        dec ebp
        jnz .L4                 

    ; check tail
    pop ecx
    and ecx,11b
    jz .L6

.L5 ; tail loop
    mov edx,[esi]
    bswap edx
    mov [edi],edx
    add esi,4
    add edi,4
    dec ecx
    jnz .L5

.L6 pop ebp
    ret







_Convert32_RGB888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov al,[esi]
    mov bl,[esi+1]
    mov dl,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov edx,edi
    and edx,11b
    jz .L4
    mov al,[esi]
    mov bl,[esi+1]
    mov dl,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jmp .L3

.L4 ; unroll 4 times
    push ebp
    mov ebp,ecx
    shr ebp,2

    ; save count
    push ecx

.L5     mov eax,[esi]                   ; first dword            eax = [A][R][G][B]
        mov ebx,[esi+4]                 ; second dword           ebx = [a][r][g][b]

        shl eax,8                       ;                        eax = [R][G][B][.]
        mov ecx,[esi+12]                ; third dword            ecx = [a][r][g][b]

        shl ebx,8                       ;                        ebx = [r][g][b][.]
        mov al,[esi+4]                  ;                        eax = [R][G][B][b]

        ror eax,8                       ;                        eax = [b][R][G][B] (done)
        mov bh,[esi+8+1]                ;                        ebx = [r][g][G][.]

        mov [edi],eax
        add edi,3*4

        shl ecx,8                       ;                        ecx = [r][g][b][.]
        mov bl,[esi+8+0]                ;                        ebx = [r][g][G][B]

        rol ebx,16                      ;                        ebx = [G][B][r][g] (done)
        mov cl,[esi+8+2]                ;                        ecx = [r][g][b][R] (done)

        mov [edi+4-3*4],ebx
        add esi,4*4
        
        mov [edi+8-3*4],ecx
        dec ebp

        jnz .L5

    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6 ; tail loop
    mov al,[esi]
    mov bl,[esi+1]
    mov dl,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jnz .L6

.L7 pop ebp
    ret







_Convert32_BGR888_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov dl,[esi]
    mov bl,[esi+1]
    mov al,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov edx,edi
    and edx,11b
    jz .L4
    mov dl,[esi]
    mov bl,[esi+1]
    mov al,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jmp .L3

.L4 ; unroll 4 times
    push ebp
    mov ebp,ecx
    shr ebp,2

    ; save count
    push ecx

.L5     mov eax,[esi]                   ; first dword            eax = [A][R][G][B]
        mov ebx,[esi+4]                 ; second dword           ebx = [a][r][g][b]
        
        bswap eax                       ;                        eax = [B][G][R][A]

        bswap ebx                       ;                        ebx = [b][g][r][a]

        mov al,[esi+4+2]                ;                        eax = [B][G][R][r] 
        mov bh,[esi+4+4+1]              ;                        ebx = [b][g][G][a]

        ror eax,8                       ;                        eax = [r][B][G][R] (done)
        mov bl,[esi+4+4+2]              ;                        ebx = [b][g][G][R]

        ror ebx,16                      ;                        ebx = [G][R][b][g] (done)
        mov [edi],eax
    
        mov [edi+4],ebx
        mov ecx,[esi+12]                ; third dword            ecx = [a][r][g][b]
        
        bswap ecx                       ;                        ecx = [b][g][r][a]
        
        mov cl,[esi+8]                  ;                        ecx = [b][g][r][B] (done)
        add esi,4*4

        mov [edi+8],ecx
        add edi,3*4

        dec ebp
        jnz .L5

    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6 ; tail loop
    mov dl,[esi]
    mov bl,[esi+1]
    mov al,[esi+2]
    mov [edi],al
    mov [edi+1],bl
    mov [edi+2],dl
    add esi,4
    add edi,3
    dec ecx
    jnz .L6

.L7 pop ebp
    ret







_Convert32_RGB565_X86:                

	; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3	; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*8]
    lea edi,[edi+ecx*4]

    ; negative counter 
    neg ecx
    jmp .L6

.L5     mov [edi+ecx*4-4],eax
.L6     mov eax,[esi+ecx*8]

        shr ah,2
        mov ebx,[esi+ecx*8+4]

        shr eax,3
        mov edx,[esi+ecx*8+4]

        shr bh,2
        mov dl,[esi+ecx*8+2]

        shl ebx,13
        and eax,000007FFh
        
        shl edx,8
        and ebx,07FF0000h

        and edx,0F800F800h
        add eax,ebx

        add eax,edx
        inc ecx

        jnz .L5                 

    mov [edi+ecx*4-4],eax

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2

.L7 ret








_Convert32_BGR565_X86:                

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3	; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*8]
    lea edi,[edi+ecx*4]

    ; negative count
    neg ecx
    jmp .L6

.L5     mov [edi+ecx*4-4],eax            
.L6     mov edx,[esi+ecx*8+4]

        mov bh,[esi+ecx*8+4]                       
        mov ah,[esi+ecx*8]                       

        shr bh,3
        mov al,[esi+ecx*8+1]             

        shr ah,3
        mov bl,[esi+ecx*8+5]           

        shl eax,3
        mov dl,[esi+ecx*8+2]

        shl ebx,19
        and eax,0000FFE0h              
                
        shr edx,3
        and ebx,0FFE00000h             
        
        and edx,001F001Fh               
        add eax,ebx

        add eax,edx
        inc ecx

        jnz .L5                 

    mov [edi+ecx*4-4],eax            

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111100b
    shl eax,3
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2

.L7 ret








_Convert32_ARGB1555_X86:                 

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3	; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*8]
    lea edi,[edi+ecx*4]

    ; negative counter 
    neg ecx
    jmp .L6

.L5     mov [edi+ecx*4-4],eax
.L6     mov eax,[esi+ecx*8]

        shr ah,3
        mov ebx,[esi+ecx*8+4]

        shr eax,3
        mov edx,[esi+ecx*8+4]

        shr bh,3
        mov dl,[esi+ecx*8+2]

        shl ebx,13
        and eax,000007FFh
        
        shl edx,7
        and ebx,07FF0000h

        and edx,07C007C00h
        add eax,ebx

        add eax,edx
        inc ecx

        jnz .L5                 

    mov [edi+ecx*4-4],eax

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov bl,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov ah,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2

.L7	ret








_Convert32_ABGR1555_X86:                

    ; check short
    cmp ecx,16
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2
    dec ecx

.L4 ; save count
    push ecx

    ; unroll twice
    shr ecx,1
    
    ; point arrays to end
    lea esi,[esi+ecx*8]
    lea edi,[edi+ecx*4]

    ; negative counter 
    neg ecx
    jmp .L6

.L5     mov [edi+ecx*4-4],eax            
.L6     mov edx,[esi+ecx*8+4]

        mov bh,[esi+ecx*8+4]                       
        mov ah,[esi+ecx*8]                       

        shr bh,3
        mov al,[esi+ecx*8+1]             

        shr ah,3
        mov bl,[esi+ecx*8+5]           

        shl eax,2
        mov dl,[esi+ecx*8+2]

        shl ebx,18
        and eax,00007FE0h              
                
        shr edx,3
        and ebx,07FE00000h             
        
        and edx,001F001Fh               
        add eax,ebx

        add eax,edx
        inc ecx

        jnz .L5                 

    mov [edi+ecx*4-4],eax            

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    mov ah,[esi+0]    ; blue
    mov al,[esi+1]    ; green
    mov bl,[esi+2]    ; red
    shr ah,3
    and al,11111000b
    shl eax,2
    shr bl,3
    add al,bl
    mov [edi+0],al
    mov [edi+1],ah
    add esi,4
    add edi,2

.L7 ret







_Convert32_GREY8_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    shr dl,2
    shr bl,1
    shr al,2
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    shr dl,2
    shr bl,1
    shr al,2
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jmp .L3

.L4 ; save count
    push ecx

    ; unroll 4 times
    shr ecx,2

.L5     mov dl,[esi+0]                                  ; blues
        mov bl,[esi+1]                                  ; greens

        mov al,[esi+2]                                  ; reds
        mov dh,[esi+4]

        shl edx,16
        mov ah,[esi+4+2]

        shl eax,16
        mov bh,[esi+4+1]

        shl ebx,16
        mov dl,[esi+4+4]

        mov bl,[esi+4+4+1]
        mov al,[esi+4+4+2]

        mov dh,[esi+4+4+4]
        mov ah,[esi+4+4+4+2]

        and eax,11111100111111001111110011111100b
        mov bh,[esi+4+4+4+1]

        ror eax,18
        and ebx,11111110111111101111111011111110b

        ror ebx,17
        and edx,11111100111111001111110011111100b

        add eax,ebx
        ror edx,18

        add edi,4
        add eax,edx

        mov [edi-4],eax
        add esi,16

        dec ecx
        jnz .L5                 

    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6    ; tail loop
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    shr dl,2
    shr bl,1
    shr al,2
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jnz .L6

.L7 ret







_Convert32_RGB332_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    and bl,11100000b
    and al,11100000b
    shr dl,6
    shr bl,3
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    mov ebx,edi
    and ebx,11b
    jz .L4   
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    and bl,11100000b
    and al,11100000b
    shr dl,6
    shr bl,3
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jmp .L3
    
.L4 ; save count
    push ecx

    ; unroll 4 times
    shr ecx,2

.L5     mov dl,[esi+0]                                  ; blues
        mov bl,[esi+1]                                  ; greens

        mov al,[esi+2]                                  ; reds
        mov dh,[esi+4]

        shl edx,16
        mov ah,[esi+4+2]

        shl eax,16
        mov bh,[esi+4+1]

        shl ebx,16
        mov dl,[esi+4+4]

        mov bl,[esi+4+4+1]
        mov al,[esi+4+4+2]

        mov dh,[esi+4+4+4]
        mov ah,[esi+4+4+4+2]

        and eax,11100000111000001110000011100000b
        mov bh,[esi+4+4+4+1]

        ror eax,16
        and ebx,11100000111000001110000011100000b

        ror ebx,19
        and edx,11000000110000001100000011000000b

        add eax,ebx
        ror edx,22

        add edi,4
        add eax,edx

        mov [edi-4],eax
        add esi,16

        dec ecx
        jnz .L5                 

    ; check tail
    pop ecx
    and ecx,11b
    jz .L7

.L6    ; tail loop
    mov dl,[esi+0]    ; blues
    mov bl,[esi+1]    ; greens
    mov al,[esi+2]    ; reds
    and bl,11100000b
    and al,11100000b
    shr dl,6
    shr bl,3
    add al,dl
    add al,bl
    mov [edi],al
    add esi,4
    inc edi
    dec ecx
    jnz .L6

.L7 ret










;----------------------------------------------------------------------------------------------------------------------------------------
; 32bit -> X area pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = pointer to area convert information            
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,ecx,edx,edi                    
;----------------------------------------------------------------------------------------------------------------------------------------


_AreaConvert32_ABGR8888_X86:                

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
        call _Convert32_ABGR8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_RGBA8888_X86:                

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
        call _Convert32_RGBA8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_BGRA8888_X86:                

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
        call _Convert32_BGRA8888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_RGB888_X86:

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
        call _Convert32_RGB888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_BGR888_X86:

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
        call _Convert32_BGR888_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_RGB565_X86:                

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
        call _Convert32_RGB565_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_BGR565_X86:                

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
        call _Convert32_BGR565_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_ARGB1555_X86:                 

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
        call _Convert32_ARGB1555_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_ABGR1555_X86:                

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
        call _Convert32_ABGR1555_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_GREY8_X86:

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
        call _Convert32_GREY8_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert32_RGB332_X86:

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
        call _Convert32_RGB332_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret
