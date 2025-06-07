option casemap:none

;include BaseMemcpy16Bit.S
;include BaseMecmpy32Bit.S
;include BaseMemcpy64Bit.S
;include BaseMemcpyScalar.S

.code

; This file implements the algorithm: 
; BASE_ASM_EXTERN DECLARE_NOALIAS void* CDECL __base_memcpy(
;   void*			destination,
;   void const*		source,
;   size_t			size)
;{ 
;	unsigned long long* destinationForWrite = reinterpret_cast<unsigned long long*>(destination);
;	const unsigned long long* sourceForRead = reinterpret_cast<const unsigned long long*>(source);
;
;	while (size--)
;		*destinationForWrite++ = *sourceForRead++;
;
;	return destination;
;}
	
dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_a(void *,void const *,unsigned __int64) PROC             ; memcpy_128bit_a, COMDAT
        mov     r10, rcx
        test    r8, r8
        je      SHORT $LN10@memcpy_128
        npad    8
$LL2@memcpy_128:
        mov     r9, rdx
        mov     rax, r10
        add     rdx, 16
        add     r10, 16
        movdqu  xmm0, XMMWORD PTR [r9]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
$LN10@memcpy_128:
        mov     rax, rcx
        ret     0
void * memcpy_128bit_a(void *,void const *,unsigned __int64) ENDP      

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_32B_a(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      SHORT $LN6@memcpy_128
$LL2@memcpy_128:
        movdqu  xmm0, XMMWORD PTR [r11]
        lea     rdx, QWORD PTR [r11+16]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     rcx, QWORD PTR [rax+16]
        movdqu  xmm0, XMMWORD PTR [rdx]
        lea     r11, QWORD PTR [r11+32]
        movdqu  XMMWORD PTR [rcx], xmm0
        lea     rax, QWORD PTR [rax+32]
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN6@memcpy_128:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_32B_a(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_64B_a(void *,void const *,unsigned __int64) PROC    
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      SHORT $LN6@memcpy_128
$LL2@memcpy_128:
        movdqu  xmm0, XMMWORD PTR [r11]
        lea     rdx, QWORD PTR [r11+48]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     rcx, QWORD PTR [rax+48]
        movdqu  xmm0, XMMWORD PTR [r11+16]
        lea     r11, QWORD PTR [r11+64]
        movdqu  XMMWORD PTR [rax+16], xmm0
        lea     rax, QWORD PTR [rax+64]
        movdqu  xmm0, XMMWORD PTR [r11-32]
        movdqu  XMMWORD PTR [rax-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx]
        movdqu  XMMWORD PTR [rcx], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN6@memcpy_128:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_64B_a(void *,void const *,unsigned __int64) ENDP     

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_128B_a(void *,void const *,unsigned __int64) PROC     
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      SHORT $LN6@memcpy_128
$LL2@memcpy_128:
        movdqu  xmm0, XMMWORD PTR [r11]
        lea     rdx, QWORD PTR [r11+112]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     rcx, QWORD PTR [rax+112]
        movdqu  xmm0, XMMWORD PTR [r11+16]
        lea     r11, QWORD PTR [r11+128]
        movdqu  XMMWORD PTR [rax+16], xmm0
        lea     rax, QWORD PTR [rax+128]
        movdqu  xmm0, XMMWORD PTR [r11-96]
        movdqu  XMMWORD PTR [rax-96], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-80]
        movdqu  XMMWORD PTR [rax-80], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-64]
        movdqu  XMMWORD PTR [rax-64], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-48]
        movdqu  XMMWORD PTR [rax-48], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-32]
        movdqu  XMMWORD PTR [rax-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx]
        movdqu  XMMWORD PTR [rcx], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN6@memcpy_128:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_128B_a(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_256B_a(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      $LN6@memcpy_128
        npad    9
$LL2@memcpy_128:
        movdqu  xmm0, XMMWORD PTR [r11]
        lea     rdx, QWORD PTR [r11+240]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     rcx, QWORD PTR [rax+240]
        movdqu  xmm0, XMMWORD PTR [r11+16]
        lea     r11, QWORD PTR [r11+256]
        movdqu  XMMWORD PTR [rax+16], xmm0
        lea     rax, QWORD PTR [rax+256]
        movdqu  xmm0, XMMWORD PTR [r11-224]
        movdqu  XMMWORD PTR [rax-224], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-208]
        movdqu  XMMWORD PTR [rax-208], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-192]
        movdqu  XMMWORD PTR [rax-192], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-176]
        movdqu  XMMWORD PTR [rax-176], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-160]
        movdqu  XMMWORD PTR [rax-160], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-144]
        movdqu  XMMWORD PTR [rax-144], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-128]
        movdqu  XMMWORD PTR [rax-128], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-112]
        movdqu  XMMWORD PTR [rax-112], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-96]
        movdqu  XMMWORD PTR [rax-96], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-80]
        movdqu  XMMWORD PTR [rax-80], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-64]
        movdqu  XMMWORD PTR [rax-64], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-48]
        movdqu  XMMWORD PTR [rax-48], xmm0
        movdqu  xmm0, XMMWORD PTR [r11-32]
        movdqu  XMMWORD PTR [rax-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx]
        movdqu  XMMWORD PTR [rcx], xmm0
        sub     r8, 1
        jne     $LL2@memcpy_128
        mov     rax, rbx
$LN6@memcpy_128:
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_256B_a(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_as(void *,void const *,unsigned __int64) PROC
        mov     r10, rcx
        test    r8, r8
        je      SHORT $LN10@memcpy_128
        npad    8
$LL2@memcpy_128:
        mov     rax, rdx
        mov     r9, r10
        add     rdx, 16
        add     r10, 16
        movntdqa xmm0, XMMWORD PTR [rax]
        movntdq XMMWORD PTR [r9], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
$LN10@memcpy_128:
        mov     rax, rcx
        sfence
        ret     0
void * memcpy_128bit_as(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_32B_as(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      SHORT $LN10@memcpy_128
        npad    13
$LL2@memcpy_128:
        lea     rcx, QWORD PTR [r11+16]
        lea     rdx, QWORD PTR [rax+16]
        movntdqa xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+32]
        movntdq XMMWORD PTR [rax], xmm0
        lea     rax, QWORD PTR [rax+32]
        movntdqa xmm0, XMMWORD PTR [rcx]
        movntdq XMMWORD PTR [rdx], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN10@memcpy_128:
        sfence
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_32B_as(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_64B_as(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      SHORT $LN10@memcpy_128
        npad    13
$LL2@memcpy_128:
        lea     rcx, QWORD PTR [r11+48]
        lea     rdx, QWORD PTR [rax+48]
        movntdqa xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+64]
        movntdq XMMWORD PTR [rax], xmm0
        lea     rax, QWORD PTR [rax+64]
        movntdqa xmm0, XMMWORD PTR [r11-48]
        movntdq XMMWORD PTR [rax-48], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-32]
        movntdq XMMWORD PTR [rax-32], xmm0
        movntdqa xmm0, XMMWORD PTR [rcx]
        movntdq XMMWORD PTR [rdx], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN10@memcpy_128:
        sfence
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_64B_as(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_128B_as(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      $LN10@memcpy_128
        npad    9
$LL2@memcpy_128:
        lea     rcx, QWORD PTR [r11+112]
        lea     rdx, QWORD PTR [rax+112]
        movntdqa xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+128]
        movntdq XMMWORD PTR [rax], xmm0
        lea     rax, QWORD PTR [rax+128]
        movntdqa xmm0, XMMWORD PTR [r11-112]
        movntdq XMMWORD PTR [rax-112], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-96]
        movntdq XMMWORD PTR [rax-96], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-80]
        movntdq XMMWORD PTR [rax-80], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-64]
        movntdq XMMWORD PTR [rax-64], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-48]
        movntdq XMMWORD PTR [rax-48], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-32]
        movntdq XMMWORD PTR [rax-32], xmm0
        movntdqa xmm0, XMMWORD PTR [rcx]
        movntdq XMMWORD PTR [rdx], xmm0
        sub     r8, 1
        jne     SHORT $LL2@memcpy_128
        mov     rax, rbx
$LN10@memcpy_128:
        sfence
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_128B_as(void *,void const *,unsigned __int64) ENDP

dest$ = 8
src$ = 16
len$ = 24
void * memcpy_128bit_256B_as(void *,void const *,unsigned __int64) PROC
$LN14:
        mov     QWORD PTR [rsp+8], rbx
        mov     r11, rdx
        mov     rbx, rcx
        mov     rax, rcx
        test    r8, r8
        je      $LN10@memcpy_128
        npad    9
$LL2@memcpy_128:
        lea     rcx, QWORD PTR [r11+240]
        lea     rdx, QWORD PTR [rax+240]
        movntdqa xmm0, XMMWORD PTR [r11]
        lea     r11, QWORD PTR [r11+256]
        movntdq XMMWORD PTR [rax], xmm0
        lea     rax, QWORD PTR [rax+256]
        movntdqa xmm0, XMMWORD PTR [r11-240]
        movntdq XMMWORD PTR [rax-240], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-224]
        movntdq XMMWORD PTR [rax-224], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-208]
        movntdq XMMWORD PTR [rax-208], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-192]
        movntdq XMMWORD PTR [rax-192], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-176]
        movntdq XMMWORD PTR [rax-176], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-160]
        movntdq XMMWORD PTR [rax-160], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-144]
        movntdq XMMWORD PTR [rax-144], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-128]
        movntdq XMMWORD PTR [rax-128], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-112]
        movntdq XMMWORD PTR [rax-112], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-96]
        movntdq XMMWORD PTR [rax-96], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-80]
        movntdq XMMWORD PTR [rax-80], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-64]
        movntdq XMMWORD PTR [rax-64], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-48]
        movntdq XMMWORD PTR [rax-48], xmm0
        movntdqa xmm0, XMMWORD PTR [r11-32]
        movntdq XMMWORD PTR [rax-32], xmm0
        movntdqa xmm0, XMMWORD PTR [rcx]
        movntdq XMMWORD PTR [rdx], xmm0
        sub     r8, 1
        jne     $LL2@memcpy_128
        mov     rax, rbx
$LN10@memcpy_128:
        sfence
        mov     rbx, QWORD PTR [rsp+8]
        ret     0
void * memcpy_128bit_256B_as(void *,void const *,unsigned __int64) ENDP

dest$ = 48
src$ = 56
numbytes$ = 64
void * memcpy_large(void *,void *,unsigned __int64) PROC
$LN125:
        mov     QWORD PTR [rsp+8], rbx
        mov     QWORD PTR [rsp+16], rbp
        mov     QWORD PTR [rsp+24], rsi
        push    rdi
        sub     rsp, 32                             ; 00000020H
        mov     rbx, r8
        mov     rdi, rdx
        mov     rsi, rcx
        mov     rbp, rcx
        test    r8, r8
        je      $LN99@memcpy_lar
        npad    7
$LL2@memcpy_lar:
        cmp     rbx, 2
        jb      $LN97@memcpy_lar
        cmp     rbx, 4
        jae     SHORT $LN6@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 1
        je      SHORT $LN64@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    15
$LL26@memcpy_lar:
        movzx   eax, WORD PTR [rcx]
        mov     WORD PTR [r8+rcx], ax
        lea     rcx, QWORD PTR [rcx+2]
        sub     rdx, 1
        jne     SHORT $LL26@memcpy_lar
$LN64@memcpy_lar:
        mov     rax, rbx
        and     rax, -2
        and     ebx, 1
        jmp     $LN19@memcpy_lar
$LN6@memcpy_lar:
        cmp     rbx, 8
        jae     SHORT $LN8@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 2
        test    rdx, rdx
        je      SHORT $LN66@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    4
$LL30@memcpy_lar:
        mov     eax, DWORD PTR [rcx]
        mov     DWORD PTR [r8+rcx], eax
        lea     rcx, QWORD PTR [rcx+4]
        sub     rdx, 1
        jne     SHORT $LL30@memcpy_lar
$LN66@memcpy_lar:
        mov     rax, rbx
        and     rax, -4
        and     ebx, 3
        jmp     $LN19@memcpy_lar
$LN8@memcpy_lar:
        cmp     rbx, 16
        jae     SHORT $LN10@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 3
        test    rdx, rdx
        je      SHORT $LN68@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    6
$LL34@memcpy_lar:
        mov     rax, QWORD PTR [rcx]
        mov     QWORD PTR [r8+rcx], rax
        lea     rcx, QWORD PTR [rcx+8]
        sub     rdx, 1
        jne     SHORT $LL34@memcpy_lar
$LN68@memcpy_lar:
        mov     rax, rbx
        and     rax, -8
        and     ebx, 7
        jmp     $LN19@memcpy_lar
$LN10@memcpy_lar:
        cmp     rbx, 32                             ; 00000020H
        jae     SHORT $LN12@memcpy_lar
        mov     rdx, rbx
        mov     r8, rdi
        shr     rdx, 4
        mov     r9, rsi
        test    rdx, rdx
        je      SHORT $LN70@memcpy_lar
        npad    8
$LL38@memcpy_lar:
        mov     rcx, r8
        mov     rax, r9
        add     r8, 16
        add     r9, 16
        lddqu   xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     rdx, 1
        jne     SHORT $LL38@memcpy_lar
$LN70@memcpy_lar:
        mov     rax, rbx
        and     rax, -16
        and     ebx, 15
        jmp     $LN19@memcpy_lar
$LN12@memcpy_lar:
        mov     rdx, rdi
        cmp     rbx, 64                             ; 00000040H
        jae     SHORT $LN14@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 5
        test    r9, r9
        je      SHORT $LN72@memcpy_lar
        npad    13
$LL42@memcpy_lar:
        lea     rcx, QWORD PTR [rdx+16]
        lea     rax, QWORD PTR [r8+16]
        lddqu   xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+32]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+32]
        lddqu   xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r9, 1
        jne     SHORT $LL42@memcpy_lar
$LN72@memcpy_lar:
        mov     rax, rbx
        and     rax, -32                      ; ffffffffffffffe0H
        and     ebx, 31
        jmp     $LN19@memcpy_lar
$LN14@memcpy_lar:
        cmp     rbx, 128                      ; 00000080H
        jae     SHORT $LN16@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 6
        test    r9, r9
        je      SHORT $LN74@memcpy_lar
        npad    2
$LL46@memcpy_lar:
        lea     rcx, QWORD PTR [rdx+48]
        lea     rax, QWORD PTR [r8+48]
        lddqu   xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+64]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+64]
        lddqu   xmm0, XMMWORD PTR [rdx-48]
        movdqu  XMMWORD PTR [r8-48], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-32]
        movdqu  XMMWORD PTR [r8-32], xmm0
        lddqu   xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r9, 1
        jne     SHORT $LL46@memcpy_lar
$LN74@memcpy_lar:
        mov     rax, rbx
        and     rax, -64                      ; ffffffffffffffc0H
        and     ebx, 63                             ; 0000003fH
        jmp     $LN19@memcpy_lar
$LN16@memcpy_lar:
        cmp     rbx, 256                      ; 00000100H
        jae     $LN18@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 7
        test    r9, r9
        je      SHORT $LN76@memcpy_lar
        npad    8
$LL50@memcpy_lar:
        lea     rcx, QWORD PTR [rdx+112]
        lea     rax, QWORD PTR [r8+112]
        lddqu   xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+128]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+128]
        lddqu   xmm0, XMMWORD PTR [rdx-112]
        movdqu  XMMWORD PTR [r8-112], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-96]
        movdqu  XMMWORD PTR [r8-96], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-80]
        movdqu  XMMWORD PTR [r8-80], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-64]
        movdqu  XMMWORD PTR [r8-64], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-48]
        movdqu  XMMWORD PTR [r8-48], xmm0
        lddqu   xmm0, XMMWORD PTR [rdx-32]
        movdqu  XMMWORD PTR [r8-32], xmm0
        lddqu   xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r9, 1
        jne     SHORT $LL50@memcpy_lar
$LN76@memcpy_lar:
        mov     rax, rbx
        and     rax, -128               ; ffffffffffffff80H
        and     ebx, 127                      ; 0000007fH
        jmp     SHORT $LN19@memcpy_lar
$LN18@memcpy_lar:
        mov     r8, rbx
        mov     rcx, rsi
        shr     r8, 8
        call    void * memcpy_128bit_256B_u(void *,void const *,unsigned __int64)
        mov     rax, rbx
        movzx   ebx, bl
        and     rax, -256               ; ffffffffffffff00H
$LN19@memcpy_lar:
        add     rdi, rax
        add     rsi, rax
        test    rbx, rbx
        jne     $LL2@memcpy_lar
        jmp     SHORT $LN99@memcpy_lar
$LN97@memcpy_lar:
        test    rbx, rbx
        je      SHORT $LN99@memcpy_lar
        sub     rsi, rdi
        npad    1
$LL22@memcpy_lar:
        movzx   eax, BYTE PTR [rdi]
        mov     BYTE PTR [rsi+rdi], al
        lea     rdi, QWORD PTR [rdi+1]
        sub     rbx, 1
        jne     SHORT $LL22@memcpy_lar
$LN99@memcpy_lar:
        mov     rbx, QWORD PTR [rsp+48]
        mov     rax, rbp
        mov     rbp, QWORD PTR [rsp+56]
        mov     rsi, QWORD PTR [rsp+64]
        add     rsp, 32                             ; 00000020H
        pop     rdi
        ret     0
void * memcpy_large(void *,void *,unsigned __int64) ENDP        

dest$ = 8
src$ = 16
numbytes$ = 24
void * memcpy_large_a(void *,void *,unsigned __int64) PROC           
$LN135:
        mov     QWORD PTR [rsp+8], rbx
        mov     QWORD PTR [rsp+16], rdi
        mov     r9, r8
        mov     r10, rdx
        mov     rbx, rcx
        mov     rdi, rcx
        test    r8, r8
        je      $LN108@memcpy_lar
        npad    1
$LL2@memcpy_lar:
        cmp     r9, 2
        jb      $LN106@memcpy_lar
        cmp     r9, 4
        jae     SHORT $LN6@memcpy_lar
        mov     rdx, r9
        mov     rcx, r10
        shr     rdx, 1
        je      SHORT $LN69@memcpy_lar
        mov     r8, rbx
        sub     r8, r10
        npad    15
$LL26@memcpy_lar:
        movzx   eax, WORD PTR [rcx]
        mov     WORD PTR [r8+rcx], ax
        lea     rcx, QWORD PTR [rcx+2]
        sub     rdx, 1
        jne     SHORT $LL26@memcpy_lar
$LN69@memcpy_lar:
        mov     rax, r9
        and     rax, -2
        and     r9d, 1
        jmp     $LN19@memcpy_lar
$LN6@memcpy_lar:
        cmp     r9, 8
        jae     SHORT $LN8@memcpy_lar
        mov     rdx, r9
        mov     rcx, r10
        shr     rdx, 2
        test    rdx, rdx
        je      SHORT $LN71@memcpy_lar
        mov     r8, rbx
        sub     r8, r10
        npad    3
$LL30@memcpy_lar:
        mov     eax, DWORD PTR [rcx]
        mov     DWORD PTR [r8+rcx], eax
        lea     rcx, QWORD PTR [rcx+4]
        sub     rdx, 1
        jne     SHORT $LL30@memcpy_lar
$LN71@memcpy_lar:
        mov     rax, r9
        and     rax, -4
        and     r9d, 3
        jmp     $LN19@memcpy_lar
$LN8@memcpy_lar:
        cmp     r9, 16
        jae     SHORT $LN10@memcpy_lar
        mov     rdx, r9
        mov     rcx, r10
        shr     rdx, 3
        test    rdx, rdx
        je      SHORT $LN73@memcpy_lar
        mov     r8, rbx
        sub     r8, r10
        npad    5
$LL34@memcpy_lar:
        mov     rax, QWORD PTR [rcx]
        mov     QWORD PTR [r8+rcx], rax
        lea     rcx, QWORD PTR [rcx+8]
        sub     rdx, 1
        jne     SHORT $LL34@memcpy_lar
$LN73@memcpy_lar:
        mov     rax, r9
        and     rax, -8
        and     r9d, 7
        jmp     $LN19@memcpy_lar
$LN10@memcpy_lar:
        cmp     r9, 32                                    ; 00000020H
        jae     SHORT $LN12@memcpy_lar
        mov     rdx, r9
        mov     r8, r10
        shr     rdx, 4
        mov     r11, rbx
        test    rdx, rdx
        je      SHORT $LN75@memcpy_lar
        npad    7
$LL38@memcpy_lar:
        mov     rcx, r8
        mov     rax, r11
        add     r8, 16
        add     r11, 16
        movdqu  xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     rdx, 1
        jne     SHORT $LL38@memcpy_lar
$LN75@memcpy_lar:
        mov     rax, r9
        and     rax, -16
        and     r9d, 15
        jmp     $LN19@memcpy_lar
$LN12@memcpy_lar:
        mov     r11, r9
        cmp     r9, 64                                    ; 00000040H
        jae     SHORT $LN14@memcpy_lar
        shr     r11, 5
        mov     rdx, r10
        mov     r8, rbx
        test    r11, r11
        je      SHORT $LN77@memcpy_lar
        npad    12
$LL42@memcpy_lar:
        movdqu  xmm0, XMMWORD PTR [rdx]
        lea     rcx, QWORD PTR [rdx+16]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     rax, QWORD PTR [r8+16]
        movdqu  xmm0, XMMWORD PTR [rcx]
        lea     rdx, QWORD PTR [rdx+32]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     r8, QWORD PTR [r8+32]
        sub     r11, 1
        jne     SHORT $LL42@memcpy_lar
$LN77@memcpy_lar:
        mov     rax, r9
        and     rax, -32                      ; ffffffffffffffe0H
        and     r9d, 31
        jmp     $LN19@memcpy_lar
$LN14@memcpy_lar:
        cmp     r9, 128                             ; 00000080H
        jae     SHORT $LN16@memcpy_lar
        shr     r11, 6
        mov     rdx, r10
        mov     r8, rbx
        test    r11, r11
        je      SHORT $LN79@memcpy_lar
        npad    1
$LL46@memcpy_lar:
        movdqu  xmm0, XMMWORD PTR [rdx]
        lea     rcx, QWORD PTR [rdx+48]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     rax, QWORD PTR [r8+48]
        movdqu  xmm0, XMMWORD PTR [rdx+16]
        lea     rdx, QWORD PTR [rdx+64]
        movdqu  XMMWORD PTR [r8+16], xmm0
        lea     r8, QWORD PTR [r8+64]
        movdqu  xmm0, XMMWORD PTR [rdx-32]
        movdqu  XMMWORD PTR [r8-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r11, 1
        jne     SHORT $LL46@memcpy_lar
$LN79@memcpy_lar:
        mov     rax, r9
        and     rax, -64                      ; ffffffffffffffc0H
        and     r9d, 63                             ; 0000003fH
        jmp     $LN19@memcpy_lar
$LN16@memcpy_lar:
        cmp     r9, 256                             ; 00000100H
        jae     $LN18@memcpy_lar
        shr     r11, 7
        mov     r8, r10
        mov     rax, rbx
        test    r11, r11
        je      SHORT $LN81@memcpy_lar
        npad    7
$LL50@memcpy_lar:
        movdqu  xmm0, XMMWORD PTR [r8]
        lea     rdx, QWORD PTR [r8+112]
        movdqu  XMMWORD PTR [rax], xmm0
        lea     rcx, QWORD PTR [rax+112]
        movdqu  xmm0, XMMWORD PTR [r8+16]
        lea     r8, QWORD PTR [r8+128]
        movdqu  XMMWORD PTR [rax+16], xmm0
        lea     rax, QWORD PTR [rax+128]
        movdqu  xmm0, XMMWORD PTR [r8-96]
        movdqu  XMMWORD PTR [rax-96], xmm0
        movdqu  xmm0, XMMWORD PTR [r8-80]
        movdqu  XMMWORD PTR [rax-80], xmm0
        movdqu  xmm0, XMMWORD PTR [r8-64]
        movdqu  XMMWORD PTR [rax-64], xmm0
        movdqu  xmm0, XMMWORD PTR [r8-48]
        movdqu  XMMWORD PTR [rax-48], xmm0
        movdqu  xmm0, XMMWORD PTR [r8-32]
        movdqu  XMMWORD PTR [rax-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx]
        movdqu  XMMWORD PTR [rcx], xmm0
        sub     r11, 1
        jne     SHORT $LL50@memcpy_lar
$LN81@memcpy_lar:
        mov     rax, r9
        and     rax, -128               ; ffffffffffffff80H
        and     r9d, 127                      ; 0000007fH
        jmp     $LN19@memcpy_lar
$LN18@memcpy_lar:
        shr     r11, 8
        mov     rdx, r10
        mov     r8, rbx
        test    r11, r11
        je      $LN83@memcpy_lar
        npad    14
$LL54@memcpy_lar:
        movdqu  xmm0, XMMWORD PTR [rdx]
        lea     rcx, QWORD PTR [rdx+240]
        movdqu  XMMWORD PTR [r8], xmm0
        lea     rax, QWORD PTR [r8+240]
        movdqu  xmm0, XMMWORD PTR [rdx+16]
        lea     rdx, QWORD PTR [rdx+256]
        movdqu  XMMWORD PTR [r8+16], xmm0
        lea     r8, QWORD PTR [r8+256]
        movdqu  xmm0, XMMWORD PTR [rdx-224]
        movdqu  XMMWORD PTR [r8-224], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-208]
        movdqu  XMMWORD PTR [r8-208], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-192]
        movdqu  XMMWORD PTR [r8-192], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-176]
        movdqu  XMMWORD PTR [r8-176], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-160]
        movdqu  XMMWORD PTR [r8-160], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-144]
        movdqu  XMMWORD PTR [r8-144], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-128]
        movdqu  XMMWORD PTR [r8-128], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-112]
        movdqu  XMMWORD PTR [r8-112], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-96]
        movdqu  XMMWORD PTR [r8-96], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-80]
        movdqu  XMMWORD PTR [r8-80], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-64]
        movdqu  XMMWORD PTR [r8-64], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-48]
        movdqu  XMMWORD PTR [r8-48], xmm0
        movdqu  xmm0, XMMWORD PTR [rdx-32]
        movdqu  XMMWORD PTR [r8-32], xmm0
        movdqu  xmm0, XMMWORD PTR [rcx]
        movdqu  XMMWORD PTR [rax], xmm0
        sub     r11, 1
        jne     $LL54@memcpy_lar
$LN83@memcpy_lar:
        mov     rax, r9
        movzx   r9d, r9b
        and     rax, -256               ; ffffffffffffff00H
$LN19@memcpy_lar:
        add     r10, rax
        add     rbx, rax
        test    r9, r9
        jne     $LL2@memcpy_lar
        mov     rax, rdi
        mov     rbx, QWORD PTR [rsp+8]
        mov     rdi, QWORD PTR [rsp+16]
        ret     0
$LN106@memcpy_lar:
        test    r9, r9
        je      SHORT $LN108@memcpy_lar
        sub     rbx, r10
        npad    9
$LL22@memcpy_lar:
        movzx   eax, BYTE PTR [r10]
        mov     BYTE PTR [rbx+r10], al
        lea     r10, QWORD PTR [r10+1]
        sub     r9, 1
        jne     SHORT $LL22@memcpy_lar
$LN108@memcpy_lar:
        mov     rbx, QWORD PTR [rsp+8]
        mov     rax, rdi
        mov     rdi, QWORD PTR [rsp+16]
        ret     0
void * memcpy_large_a(void *,void *,unsigned __int64) ENDP            

dest$ = 48
src$ = 56
numbytes$ = 64
void * memcpy_large_as(void *,void *,unsigned __int64) PROC             
$LN125:
        mov     QWORD PTR [rsp+8], rbx
        mov     QWORD PTR [rsp+16], rbp
        mov     QWORD PTR [rsp+24], rsi
        push    rdi
        sub     rsp, 32                             ; 00000020H
        mov     rbx, r8
        mov     rdi, rdx
        mov     rsi, rcx
        mov     rbp, rcx
        test    r8, r8
        je      $LN99@memcpy_lar
        npad    7
$LL2@memcpy_lar:
        cmp     rbx, 2
        jb      $LN97@memcpy_lar
        cmp     rbx, 4
        jae     SHORT $LN6@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 1
        je      SHORT $LN64@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    15
$LL26@memcpy_lar:
        movzx   eax, WORD PTR [rcx]
        mov     WORD PTR [r8+rcx], ax
        lea     rcx, QWORD PTR [rcx+2]
        sub     rdx, 1
        jne     SHORT $LL26@memcpy_lar
$LN64@memcpy_lar:
        mov     rax, rbx
        and     rax, -2
        and     ebx, 1
        jmp     $LN19@memcpy_lar
$LN6@memcpy_lar:
        cmp     rbx, 8
        jae     SHORT $LN8@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 2
        test    rdx, rdx
        je      SHORT $LN66@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    4
$LL30@memcpy_lar:
        mov     eax, DWORD PTR [rcx]
        mov     DWORD PTR [r8+rcx], eax
        lea     rcx, QWORD PTR [rcx+4]
        sub     rdx, 1
        jne     SHORT $LL30@memcpy_lar
$LN66@memcpy_lar:
        mov     rax, rbx
        and     rax, -4
        and     ebx, 3
        jmp     $LN19@memcpy_lar
$LN8@memcpy_lar:
        cmp     rbx, 16
        jae     SHORT $LN10@memcpy_lar
        mov     rdx, rbx
        mov     rcx, rdi
        shr     rdx, 3
        test    rdx, rdx
        je      SHORT $LN68@memcpy_lar
        mov     r8, rsi
        sub     r8, rdi
        npad    6
$LL34@memcpy_lar:
        mov     rax, QWORD PTR [rcx]
        mov     QWORD PTR [r8+rcx], rax
        lea     rcx, QWORD PTR [rcx+8]
        sub     rdx, 1
        jne     SHORT $LL34@memcpy_lar
$LN68@memcpy_lar:
        mov     rax, rbx
        and     rax, -8
        and     ebx, 7
        jmp     $LN19@memcpy_lar
$LN10@memcpy_lar:
        cmp     rbx, 32                             ; 00000020H
        jae     SHORT $LN12@memcpy_lar
        mov     rdx, rbx
        mov     r8, rdi
        shr     rdx, 4
        mov     r9, rsi
        test    rdx, rdx
        je      SHORT $LN70@memcpy_lar
        npad    8
$LL38@memcpy_lar:
        mov     rax, r8
        mov     rcx, r9
        add     r8, 16
        add     r9, 16
        movntdqa xmm0, XMMWORD PTR [rax]
        movntdq XMMWORD PTR [rcx], xmm0
        sub     rdx, 1
        jne     SHORT $LL38@memcpy_lar
$LN70@memcpy_lar:
        sfence
        mov     rax, rbx
        and     rax, -16
        and     ebx, 15
        jmp     $LN19@memcpy_lar
$LN12@memcpy_lar:
        mov     rdx, rdi
        cmp     rbx, 64                             ; 00000040H
        jae     SHORT $LN14@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 5
        test    r9, r9
        je      SHORT $LN72@memcpy_lar
        npad    9
$LL42@memcpy_lar:
        lea     rax, QWORD PTR [rdx+16]
        lea     rcx, QWORD PTR [r8+16]
        movntdqa xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+32]
        movntdq XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+32]
        movntdqa xmm0, XMMWORD PTR [rax]
        movntdq XMMWORD PTR [rcx], xmm0
        sub     r9, 1
        jne     SHORT $LL42@memcpy_lar
$LN72@memcpy_lar:
        sfence
        mov     rax, rbx
        and     rax, -32                      ; ffffffffffffffe0H
        and     ebx, 31
        jmp     $LN19@memcpy_lar
$LN14@memcpy_lar:
        cmp     rbx, 128                      ; 00000080H
        jae     SHORT $LN16@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 6
        test    r9, r9
        je      SHORT $LN74@memcpy_lar
$LL46@memcpy_lar:
        lea     rax, QWORD PTR [rdx+48]
        lea     rcx, QWORD PTR [r8+48]
        movntdqa xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+64]
        movntdq XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+64]
        movntdqa xmm0, XMMWORD PTR [rdx-48]
        movntdq XMMWORD PTR [r8-48], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-32]
        movntdq XMMWORD PTR [r8-32], xmm0
        movntdqa xmm0, XMMWORD PTR [rax]
        movntdq XMMWORD PTR [rcx], xmm0
        sub     r9, 1
        jne     SHORT $LL46@memcpy_lar
$LN74@memcpy_lar:
        sfence
        mov     rax, rbx
        and     rax, -64                      ; ffffffffffffffc0H
        and     ebx, 63                             ; 0000003fH
        jmp     $LN19@memcpy_lar
$LN16@memcpy_lar:
        cmp     rbx, 256                      ; 00000100H
        jae     $LN18@memcpy_lar
        mov     r9, rbx
        mov     r8, rsi
        shr     r9, 7
        test    r9, r9
        je      SHORT $LN76@memcpy_lar
$LL50@memcpy_lar:
        lea     rax, QWORD PTR [rdx+112]
        lea     rcx, QWORD PTR [r8+112]
        movntdqa xmm0, XMMWORD PTR [rdx]
        lea     rdx, QWORD PTR [rdx+128]
        movntdq XMMWORD PTR [r8], xmm0
        lea     r8, QWORD PTR [r8+128]
        movntdqa xmm0, XMMWORD PTR [rdx-112]
        movntdq XMMWORD PTR [r8-112], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-96]
        movntdq XMMWORD PTR [r8-96], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-80]
        movntdq XMMWORD PTR [r8-80], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-64]
        movntdq XMMWORD PTR [r8-64], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-48]
        movntdq XMMWORD PTR [r8-48], xmm0
        movntdqa xmm0, XMMWORD PTR [rdx-32]
        movntdq XMMWORD PTR [r8-32], xmm0
        movntdqa xmm0, XMMWORD PTR [rax]
        movntdq XMMWORD PTR [rcx], xmm0
        sub     r9, 1
        jne     SHORT $LL50@memcpy_lar
$LN76@memcpy_lar:
        sfence
        mov     rax, rbx
        and     rax, -128               ; ffffffffffffff80H
        and     ebx, 127                      ; 0000007fH
        jmp     SHORT $LN19@memcpy_lar
$LN18@memcpy_lar:
        mov     r8, rbx
        mov     rcx, rsi
        shr     r8, 8
        call    void * memcpy_128bit_256B_as(void *,void const *,unsigned __int64)
        mov     rax, rbx
        movzx   ebx, bl
        and     rax, -256               ; ffffffffffffff00H
$LN19@memcpy_lar:
        add     rdi, rax
        add     rsi, rax
        test    rbx, rbx
        jne     $LL2@memcpy_lar
        jmp     SHORT $LN99@memcpy_lar
$LN97@memcpy_lar:
        test    rbx, rbx
        je      SHORT $LN99@memcpy_lar
        sub     rsi, rdi
        npad    4
$LL22@memcpy_lar:
        movzx   eax, BYTE PTR [rdi]
        mov     BYTE PTR [rsi+rdi], al
        lea     rdi, QWORD PTR [rdi+1]
        sub     rbx, 1
        jne     SHORT $LL22@memcpy_lar
$LN99@memcpy_lar:
        mov     rbx, QWORD PTR [rsp+48]
        mov     rax, rbp
        mov     rbp, QWORD PTR [rsp+56]
        mov     rsi, QWORD PTR [rsp+64]
        add     rsp, 32                             ; 00000020H
        pop     rdi
        ret     0
void * memcpy_large_as(void *,void *,unsigned __int64) ENDP             

dest$ = 48
src$ = 56
numbytes$ = 64
void * __base_memcpy(void *,void *,unsigned __int64) PROC                 
$LN19:
        mov     QWORD PTR [rsp+16], rbx
        mov     QWORD PTR [rsp+24], rsi
        push    rdi
        sub     rsp, 32                             ; 00000020H
        mov     rdi, r8
        mov     rsi, rdx
        mov     rbx, rcx
        cmp     rdx, rcx
        je      SHORT $LN17@__base_memcpy
        movzx   eax, bl
        or      al, sil
        test    al, 15
        jne     SHORT $LN3@__base_memcpy
        cmp     r8, 3145728                         ; 00300000H
        jbe     SHORT $LN5@__base_memcpy
        call    void * memcpy_large_as(void *,void *,unsigned __int64)       
        mov     rax, rbx
        mov     rbx, QWORD PTR [rsp+56]
        mov     rsi, QWORD PTR [rsp+64]
        add     rsp, 32                             ; 00000020H
        pop     rdi
        ret     0
$LN5@__base_memcpy:
        call    void * memcpy_large_a(void *,void *,unsigned __int64)       
        mov     rax, rbx
        mov     rbx, QWORD PTR [rsp+56]
        mov     rsi, QWORD PTR [rsp+64]
        add     rsp, 32                             ; 00000020H
        pop     rdi
        ret     0
$LN3@__base_memcpy:
        mov     rax, rbx
        mov     QWORD PTR [rsp+48], r14
        and     eax, 15
        mov     r14d, 16
        sub     r14, rax
        cmp     rdi, r14
        jbe     SHORT $LN7@__base_memcpy
        mov     r8, r14
        call    void * memcpy_large(void *,void *,unsigned __int64)     
        sub     rdi, r14
        lea     rdx, QWORD PTR [r14+rsi]
        mov     r8, rdi
        lea     rcx, QWORD PTR [r14+rbx]
$LN7@__base_memcpy:
        call    void * memcpy_large(void *,void *,unsigned __int64)     
        mov     r14, QWORD PTR [rsp+48]
$LN17@__base_memcpy:
        mov     rsi, QWORD PTR [rsp+64]
        mov     rax, rbx
        mov     rbx, QWORD PTR [rsp+56]
        add     rsp, 32                             ; 00000020H
        pop     rdi
        ret     0
void * __base_memcpy(void *,void *,unsigned __int64) ENDP               

END
