option casemap:none

.code

dest$ = 8
src$ = 16
len$ = 24

__base_memcpy128BitUnaligned proc
        mov     r10, rcx
        test    r8, r8

        ; ===================================================================

        je      SHORT EndPart
        ; npad 8
MainPart:
        mov     r9, rdx
        mov     rax, r10

        ; ===================================================================

        add     rdx, 16
        add     r10, 16
        
        ; ===================================================================

        lddqu   xmm0, XMMWORD PTR [r9]
        movdqu  XMMWORD PTR [rax], xmm0

        ; ===================================================================

        sub     r8, 1
        jne     SHORT MainPart
EndPart:
        mov     rax, rcx
        ret     0
__base_memcpy128BitUnaligned endp

end