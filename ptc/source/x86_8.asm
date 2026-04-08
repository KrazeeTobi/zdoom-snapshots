;----------------------------------------------------------------------------------------------------------------------------------------
; 8bit -> X pixel format conversions routines for PTC (intel x86)
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

GLOBAL _Convert8_4BYTE_X86
GLOBAL _Convert8_3BYTE_X86
GLOBAL _Convert8_2BYTE_X86
GLOBAL _Convert8_1BYTE_X86
GLOBAL _AreaConvert8_4BYTE_X86
GLOBAL _AreaConvert8_3BYTE_X86
GLOBAL _AreaConvert8_2BYTE_X86
GLOBAL _AreaConvert8_1BYTE_X86

SECTION .text









_Convert8_4BYTE_X86:

    ; zero
    cmp ecx,0
    je .L3

    ; save ebp
    push ebp

    ; setup counter
    mov ebp,ecx
    neg ebp

    ; point arrays to end
    lea esi,[esi+ecx*1]
    lea edi,[edi+ecx*4]

    ; setup
    xor eax,eax

.L1     mov al,[esi+ebp*1]
        mov edx,[ebx+eax*4]
        mov [edi+ebp*4],edx
        inc ebp
        jnz .L1          

.L2 ; restore ebp
    pop ebp
.L3 ret










_Convert8_3BYTE_X86:

    ; clear edx
    xor edx,edx

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1    ; short loop
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jnz .L1
.L2 ret
    
.L3 ; head
    mov eax,edi
    and eax,11b
    jz .L4
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jmp .L3

.L4 ; save ebp
    push ebp
    mov ebp,ebx

    ; save count
    push ecx

    ; unroll 4 times
    shr ecx,2

.L5     push ecx                        ; save ecx
        mov dl,[esi]                    ; index to "A"           

        mov eax,[ebp+edx*4]             ; eax = [xx][A2][A1][A0]
        shl eax,8                       ; eax = [A2][A1][A0][xx]

        mov dl,[esi+1]                  ; index to "B"

        mov al,[ebp+edx*4+0]            ; eax = [A2][A1][A0][B0]
        ror eax,8                       ; eax = [B0][A2][A1][A0] (done)
        mov [edi],eax

        mov eax,[ebp+edx*4]             ; eax = [xx][B2][B1][B0]
        shl eax,8                       ; eax = [B2][B1][B0][xx]

        mov dl,[esi+3]                  ; index to "D"

        mov ecx,[ebp+edx*4]             ; ecx = [xx][D2][D1][D0]
        shl ecx,8                       ; ecx = [D2][D1][D0][xx]

        mov dl,[esi+2]                  ; index to "C"

        mov ah,[ebp+edx*4+1]            ; eax = [B2][B1][C1][xx]
        mov al,[ebp+edx*4+0]            ; eax = [B2][B1][C1][C0]
        ror eax,16                      ; eax = [C1][C0][B2][B1] (done)
        
        mov cl,[ebp+edx*4+2]            ; ecx = [D2][D1][D0][C2] (done)

        mov [edi+4],eax        
        mov [edi+8],ecx
    
        add esi,4
        add edi,3*4

        pop ecx                         ; restore ecx
        
        dec ecx
        jnz .L5

    ; tail
    pop ecx
    and ecx,11b
    jz .L7
.L6 mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jnz .L6

.L7 pop ebp
    ret







_Convert8_3BYTE_X86_PAIRED:              ; why is this slower than the non-paired version???!

    ; clear edx
    xor edx,edx

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

.L1 ; short loop
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jnz .L1
.L2 ret
    
.L3 ; head
    mov eax,edi
    and eax,11b
    jz .L4
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jmp .L3

.L4 ; save ebp
    push ebp
    mov ebp,ebx

    ; save count
    push ecx

    ; clear ebx
    xor ebx,ebx

    ; unroll 4 times
    shr ecx,2

    ; prestep
    mov bl,[esi]
    mov dl,[esi+1]

.L5     mov eax,[ebp+ebx*4]             ; eax = [xx][A2][A1][A0]
        mov bl,[esi+3]                  ; index to "D"

        shl eax,8                       ; eax = [A2][A1][A0][xx]
        push ecx                        ; save ecx

        mov al,[ebp+edx*4+0]            ; eax = [A2][A1][A0][B0]
        mov ecx,[ebp+ebx*4]             ; ecx = [xx][D2][D1][D0]

        ror eax,8                       ; eax = [B0][A2][A1][A0] (done)
        mov bl,[esi+4]                  ; index to "A"           

        shl ecx,8                       ; ecx = [D2][D1][D0][xx]
        mov [edi],eax

        mov eax,[ebp+edx*4]             ; eax = [xx][B2][B1][B0]
        mov dl,[esi+2]                  ; index to "C"

        shl eax,8                       ; eax = [B2][B1][B0][xx]
        add edi,3*4

        mov ah,[ebp+edx*4+1]            ; eax = [B2][B1][C1][xx]
        mov cl,[ebp+edx*4+2]            ; ecx = [D2][D1][D0][C2] (done)

        mov al,[ebp+edx*4+0]            ; eax = [B2][B1][C1][C0]
        mov [edi+8-3*4],ecx

        ror eax,16                      ; eax = [C1][C0][B2][B1] (done)
        mov dl,[esi+4+1]                ; index to "B"
        
        mov [edi+4-3*4],eax        
        add esi,4

        pop ecx                         ; restore ecx
        
        dec ecx
        jnz .L5

    ; tail
    pop ecx
    and ecx,11b
    jz .L7
.L6 mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al  ; blue
    mov [edi+1],ah  ; green
    shr eax,16
    mov [edi+2],al  ; red
    inc esi
    add edi,3
    dec ecx
    jnz .L6

.L7 pop ebp
    ret







_Convert8_2BYTE_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
.L1 xor edx,edx
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al
    mov [edi+1],ah
    inc esi
    add edi,2
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    xor edx,edx
	mov eax,edi
    and eax,11b
    jz .L4   
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al
    mov [edi+1],ah
    inc esi
    add edi,2
    dec ecx

.L4 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll two times
    mov ebp,ecx
    shr ebp,1
         
    ; point arrays to end
    lea esi,[esi+ebp*2]
    lea edi,[edi+ebp*4]

    ; negative counter 
    neg ebp

    ; setup
    xor ecx,ecx
    jmp .L6

.L5     mov eax,[ebx+ecx*4]                
        mov ecx,[ebx+edx*4]

        shl ecx,16
        add eax,ecx

        mov [edi+ebp*4-4],eax
        xor ecx,ecx

.L6     mov cl,[esi+ebp*2]
        mov dl,[esi+ebp*2+1]

        inc ebp
        jnz .L5          

    mov eax,[ebx+ecx*4]
    mov ecx,[ebx+edx*4]

    shl ecx,16
    add eax,ecx

    mov [edi+ebp*4-4],eax

    ; tail
    pop ecx
    and ecx,1
    jz .L7
    xor edx,edx
    mov dl,[esi]
    mov eax,[ebx+edx*4]
    mov [edi+0],al
    mov [edi+1],ah
    inc esi
    add edi,2

.L7 ; restore ebp
    pop ebp
    ret







_Convert8_1BYTE_X86:

    ; check short
    cmp ecx,32
    ja .L3

    ; check zero
    cmp ecx,0
    je .L2

    ; short loop
.L1 xor edx,edx
    mov dl,[esi]
    mov al,[ebx+edx]
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jnz .L1
.L2 ret

.L3 ; head
    xor edx,edx
.L4 mov eax,edi
    and eax,11b
    jz .L5   
    mov dl,[esi]
    mov al,[ebx+edx]
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jmp .L4

.L5 ; save ebp
    push ebp

    ; save count
    push ecx

    ; unroll four times
    mov ebp,ecx
    shr ebp,2
         
    ; point arrays to end
    lea esi,[esi+ebp*4]
    lea edi,[edi+ebp*4]

    ; negative counter 
    neg ebp

    ; setup
    xor ecx,ecx
    xor edx,edx

.L6     mov dl,[esi+ebp*4+3]
        mov cl,[esi+ebp*4+2]
        mov ah,[ebx+edx]
        mov al,[ebx+ecx]
        shl eax,16

        mov dl,[esi+ebp*4+1]
        mov cl,[esi+ebp*4+0]
        mov ah,[ebx+edx]
        mov al,[ebx+ecx]

        mov [edi+ebp*4],eax

        inc ebp
        jnz .L6          

.L7 ; check tail
    pop ecx
    and ecx,11b
    jz .L9

    ; tail loop
    xor edx,edx
.L8 mov dl,[esi]
    mov al,[ebx+edx]
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jnz .L8
    
.L9 ; restore ebp
    pop ebp
    ret









;----------------------------------------------------------------------------------------------------------------------------------------
; 8bit -> X area pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = pointer to area convert information
; ebx = pointer to extra data (256 index color table)            
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ecx,edx,edi                    
;----------------------------------------------------------------------------------------------------------------------------------------


_AreaConvert8_4BYTE_X86:

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
        call _Convert8_4BYTE_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert8_3BYTE_X86:

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
        call _Convert8_3BYTE_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert8_2BYTE_X86:

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
        call _Convert8_2BYTE_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret







_AreaConvert8_1BYTE_X86:

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
        call _Convert8_1BYTE_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret