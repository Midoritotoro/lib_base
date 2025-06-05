option casemap:none

.code

dest$ = 8
src$ = 16
len$ = 24

void * __base_memcpy64Bit(void *,void const *,unsigned __int64) proc
        test    r8, r8
        je      SHORT $LN10@__base_memcpy64Bit
        
        ; =======================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; =======================================================================

        npad    5
$LL2@__base_memcpy64Bit:
        mov     rax, QWORD PTR [rdx]
        mov     QWORD PTR [r9+rdx], rax

        ; ========================================================================

        lea     rdx, QWORD PTR [rdx+8]
        sub     r8, 1
        jne     SHORT $LL2@__base_memcpy64Bit
$LN10@__base_memcpy64Bit:
        mov     rax, rcx
        ret     0
void * __base_memcpy64Bit(void *,void const *,unsigned __int64) endp

end