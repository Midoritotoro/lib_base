option casemap:none

.code

dest$ = 8
src$ = 16
len$ = 24
void * __base_memcpy16Bit(void *,void const *,unsigned __int64) proc
        test    r8, r8
        je      SHORT $LN10@memcpy_16b

        ; ==================================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; ==================================================================================

        npad    5
$LL2@__base_memcpy16Bit:
        movzx   eax, WORD PTR [rdx]
        mov     WORD PTR [r9+rdx], ax

        ; ==================================================================================

        lea     rdx, QWORD PTR [rdx+2]
        sub     r8, 1

        ; ==================================================================================

        jne     SHORT $LL2@memcpy_16b
$LN10@__base_memcpy16Bit:
        mov     rax, rcx
        ret     0
void * __base_memcpy16Bit(void *,void const *,unsigned __int64) endp

end