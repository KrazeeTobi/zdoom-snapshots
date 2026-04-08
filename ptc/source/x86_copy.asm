;----------------------------------------------------------------------------------------------------------------------------------------
; "copy convert" pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = source offset              
; edi = destination offset         
; ecx = number of bytes to copy (not pixels)
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,edx                      
;----------------------------------------------------------------------------------------------------------------------------------------

BITS 32

GLOBAL _ConvertCopy_X86
GLOBAL _AreaConvertCopy_X86

SECTION .text










_ConvertCopy_X86:
;_ConvertCopy_INTEGER_X86:

    ; check short
    cmp ecx,32
    ja .L1

    ; short case
    rep movsb
    ret

.L1 ; head
    mov ebx,edi
    and ebx,11b
    jz .L2
    mov al,[esi]
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jmp .L1

.L2 ; setup ebp
    push ebp
    mov ebp,ecx

    ; unroll 16 time
    shr ebp,4
    jz .L5
    
    ; save ecx
    push ecx

.L3 ; copy body
    mov eax,[esi]
    mov ebx,[esi+4]
    mov ecx,[esi+8]
    mov edx,[esi+12]
    mov [edi],eax
    mov [edi+4],ebx
    mov [edi+8],ecx
    mov [edi+12],edx
    add esi,16
    add edi,16
    dec ebp 
    jnz .L3

.L4 ; tail
    pop ecx
    and ecx,1111b
    rep movsb

.L5 ; done
    pop ebp
    ret






_ConvertCopy_MOVSD_X86:

    ; check short
    cmp ecx,16
    ja .L1

    ; short case
    rep movsb
    ret

.L1 ; head
    mov ebx,edi
    and ebx,11b
    jz .L2
    mov al,[esi]
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jmp .L1

.L2 ; body
    mov edx,ecx
    shr ecx,2
    rep movsd

    ; tail
    and edx,11b
    jz .L4
.L3 mov al,[esi]
    mov [edi],al
    inc esi
    inc edi
    dec edx
    jnz .L3

.L4 ret








;----------------------------------------------------------------------------------------------------------------------------------------
; "area copy convert" pixel format conversions routines for PTC (intel x86)
;----------------------------------------------------------------------------------------------------------------------------------------
; PARAMETERS                       
; esi = pointer to area convert information            
;----------------------------------------------------------------------------------------------------------------------------------------
; MODIFY                           
; eax,ebx,ecx,edx,edi                    
;----------------------------------------------------------------------------------------------------------------------------------------


_AreaConvertCopy_X86:

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
        call _ConvertCopy_X86
        add esi,[ebp+32]
        add edi,[ebp+36]
        dec dword [ebp+44]
        jnz .L1

.L2 ; restore ebp
    pop ebp

    ret
