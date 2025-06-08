option casemap:none

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

end
