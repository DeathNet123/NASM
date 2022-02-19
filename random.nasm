section .data
msg: db "Hello world", 0xA
msglen equ $ - msg
var: db 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8
section .bss
;let's keep it empty for while
section .text
global _start
_start:
mov rax, [var]
mov rax, 60
mov rdi, 64
syscall