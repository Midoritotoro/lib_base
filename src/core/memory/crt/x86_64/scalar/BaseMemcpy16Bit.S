option casemap:none

.code

; Declaration in C++
; BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpy16Bit(void* destination, const void* source, size_t length) noexcept;

__base_memcpy16Bit proc
        test    r8, r8
        je      SHORT EndPart

        ; ==================================================================================

        mov     r9, rcx
        sub     r9, rdx

        ; ==================================================================================

        ;npad    5
MainPart:
        movzx   eax, WORD PTR [rdx]
        mov     WORD PTR [r9+rdx], ax

        ; ==================================================================================

        lea     rdx, QWORD PTR [rdx+2]
        sub     r8, 1

        ; ==================================================================================

        jne     SHORT MainPart
EndPart:
        mov     rax, rcx
        ret     0
__base_memcpy16Bit endp

end