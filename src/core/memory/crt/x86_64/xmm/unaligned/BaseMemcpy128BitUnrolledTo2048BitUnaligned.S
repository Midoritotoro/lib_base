option casemap:none 

.code

dest$ = 8
src$ = 16
len$ = 24

; Copies 256 bytes at a time 

; void* __base_memcpy128BitUnrolledTo2048BitUnaligned(void* destination, const void* source, size_t length);


__base_memcpy128BitUnrolledTo2048BitUnaligned proc
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        
        ; ========================================================================

        test    r8, r8
        je      $LN6@__base_memcpy128BitUnrolledTo2048BitUnaligned
        
        ; ========================================================================

        npad    9
$LL2@__base_memcpy128BitUnrolledTo2048BitUnaligned:
        lea     rdx, QWORD PTR [r11+240]
        lea     rcx, QWORD PTR [rax+240]
        
        ; ========================================================================
        
        lddqu   xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+256]
        movdqu  XMMWORD PTR [rax], xmm0
        
        ; ========================================================================

        lea     rax, QWORD PTR [rax+256]
        lddqu   xmm0, XMMWORD PTR [r11-240]
        movdqu  XMMWORD PTR [rax-240], xmm0
        
        ; ========================================================================

        lddqu   xmm0, XMMWORD PTR [r11-224]
        movdqu  XMMWORD PTR [rax-224], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-208]
        
        ; ========================================================================

        movdqu  XMMWORD PTR [rax-208], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-192]
        movdqu  XMMWORD PTR [rax-192], xmm0
        
        ; ========================================================================

        lddqu   xmm0, XMMWORD PTR [r11-176]
        movdqu  XMMWORD PTR [rax-176], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-160]
        
        ; ========================================================================

        movdqu  XMMWORD PTR [rax-160], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-144]
        movdqu  XMMWORD PTR [rax-144], xmm0

        ; ========================================================================

        lddqu   xmm0, XMMWORD PTR [r11-128]
        movdqu  XMMWORD PTR [rax-128], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-112]
        
        ; ========================================================================

        movdqu  XMMWORD PTR [rax-112], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-96]
        movdqu  XMMWORD PTR [rax-96], xmm0
        
        ; ========================================================================

        lddqu   xmm0, XMMWORD PTR [r11-80]
        movdqu  XMMWORD PTR [rax-80], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-64]
        
        ; ========================================================================

        movdqu  XMMWORD PTR [rax-64], xmm0
        lddqu   xmm0, XMMWORD PTR [r11-48]
        movdqu  XMMWORD PTR [rax-48], xmm0
        
        ; ========================================================================

        lddqu   xmm0, XMMWORD PTR [r11-32]
        movdqu  XMMWORD PTR [rax-32], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx]
        
        ; ========================================================================

        movdqu  XMMWORD PTR [rcx], xmm0
        
        ; ========================================================================

        sub     r8, 1
        jne     $LL2@memcpy_128
        
        ; ========================================================================

        mov     rax, rbx
$LN6@__base_memcpy128BitUnrolledTo2048BitUnaligned:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
__base_memcpy128BitUnrolledTo2048BitUnaligned endp


end