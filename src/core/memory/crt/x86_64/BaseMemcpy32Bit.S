option casemap:none

.code

dest$ = 8
src$ = 16
len$ = 24
void * __base_memcpy32Bit(void *,void const *,unsigned __int64) proc
        test    r8, r8
        je      SHORT $LN10@__base_memcpy32Bit

        ; ================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; ================================================================

        npad    5
$LL2@__base_memcpy32Bit:
        mov     eax, DWORD PTR [rdx]
        mov     DWORD PTR [r9+rdx], eax

        ; ================================================================

        lea     rdx, QWORD PTR [rdx+4]
        sub     r8, 1

        ; ================================================================

        jne     SHORT $LL2@__base_memcpy32Bit
$LN10@__base_memcpy32Bit:
        mov     rax, rcx
        ret     0
void * __base_memcpy32Bit(void *,void const *,unsigned __int64) endp


end