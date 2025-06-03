option casemap:none

.386
.model flat, c
option casemap:none


.code

start:

memcpy proc public

ZmmMove64:

YmmMove32:

XmmMove16:

memcpy endp

end start


end