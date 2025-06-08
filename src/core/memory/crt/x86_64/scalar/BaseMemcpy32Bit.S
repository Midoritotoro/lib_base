option casemap:none

.code

dest$ = 8
src$ = 16
len$ = 24

__base_memcpy32Bit proc
        test    r8, r8
        je      SHORT EndPart

        ; ================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; ================================================================

;        npad    5
MainPart:
        mov     eax, DWORD PTR [rdx]
        mov     DWORD PTR [r9+rdx], eax

        ; ================================================================

        lea     rdx, QWORD PTR [rdx+4]
        sub     r8, 1

        ; ================================================================

        jne     SHORT MainPart
EndPart:
        mov     rax, rcx
        ret     0
__base_memcpy32Bit endp


end