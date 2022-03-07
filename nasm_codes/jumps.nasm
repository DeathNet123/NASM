section .data
msg: db "I love Pakistan", 0xA
MSG_LEN equ $ - msg
section .text
global _start:
_start:
mov rbx, 12
_loop:
mov rax, 1
mov rdi, 1
mov rsi, msg
mov rdx, MSG_LEN
syscall
sub rbx, 1
cmp rbx, 0d
jg _loop
_end:
mov rax, 60
mov rdi, 0
syscall