option casemap:none

.code 

dest$ = 8
src$ = 16
len$ = 24

; Copies 64 bytes at a time

__base_memcpy128BitUnrolledTo512BitUnaligned proc

StartPart:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx

        ; =================================================================================

        test    r8, r8
        je      SHORT EndPart
MainPart:
        lea     rdx, QWORD PTR [r11+48]
        lea     rcx, QWORD PTR [rax+48]

        ; =================================================================================
        
        lddqu   xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+64]
        movdqu  XMMWORD PTR [rax], xmm0

        ; =================================================================================

        lea     rax, QWORD PTR [rax+64]
        lddqu   xmm0, XMMWORD PTR [r11-48]
        movdqu  XMMWORD PTR [rax-48], xmm0

        ; =================================================================================

        lddqu   xmm0, XMMWORD PTR [r11-32]
        movdqu  XMMWORD PTR [rax-32], xmm0

        ; =================================================================================

        lddqu   xmm0, XMMWORD PTR [rdx]
        movdqu  XMMWORD PTR [rcx], xmm0

        ; =================================================================================

        sub     r8, 1
        jne     SHORT MainPart
        mov     rax, rbx
EndPart:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
__base_memcpy128BitUnrolledTo512BitUnaligned endp

end
