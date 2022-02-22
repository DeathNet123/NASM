section .data
msg: db "I love Pakistan", 0xA
msg_len equ $ - msg
section .text
global _start
_start:
mov rcx, 100
_loop:
mov rax, 1
mov rdi, 1
mov rsi, msg
mov rdx, msg_len
push rcx ; saving the rcx value to stay safe
syscall
pop rcx
loop _loop
_end:
mov rax, 60
mov rdi, 0
syscall