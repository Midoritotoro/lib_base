option casemap:none


; Scalar _base_memcpy version

.code

dest$ = 8
src$ = 16
len$ = 24

void * __base_memcpy(void *,void const *,unsigned __int64) proc
        test    r8, r8
        je      SHORT $LN10@__base_memcpy

        ; ==================================================================

        mov     r9, rcx
        sub     r9, rdx
        npad    5
$LL2@__base_memcpy:
        movzx   eax, BYTE PTR [rdx]
        mov     BYTE PTR [r9+rdx], al

        ; ==================================================================

        lea     rdx, QWORD PTR [rdx+1]
        sub     r8, 1

        ; ==================================================================

        jne     SHORT $LL2@__base_memcpy
$LN10@__base_memcpy:
        mov     rax, rcx
        ret     0
void * __base_memcpy(void *,void const *,unsigned __int64) endp   

end