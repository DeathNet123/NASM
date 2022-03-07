section .data
var: db 1, 2, 3
section .text
global _start
_start:
mov r11b, byte [var]
mov rax, 60
mov rdi, 0
syscall