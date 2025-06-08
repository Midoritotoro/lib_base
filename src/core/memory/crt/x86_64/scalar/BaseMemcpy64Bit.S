option casemap:none

.code

; Declaration in C++
; BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpy64Bit(void* destination, const void* source, size_t length) noexcept;

__base_memcpy64Bit proc
        test    r8, r8
        je      SHORT EndPart
        
        ; =======================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; =======================================================================

;        npad    5
MainPart:
        mov     rax, QWORD PTR [rdx]
        mov     QWORD PTR [r9+rdx], rax

        ; ========================================================================

        lea     rdx, QWORD PTR [rdx+8]
        sub     r8, 1
        jne     SHORT MainPart
EndPart:
        mov     rax, rcx
        ret     0
__base_memcpy64Bit endp

end