section .data
msg: db "Hi, this is me!", 0xA
MSG_LEN equ $ - msg 
section .text
global _start
_start:
mov rcx, 10d
_repeat:
push rcx
call print
pop rcx
loop _repeat
_end:
mov rax, 60
mov rdi, 54
syscall
print:
mov rax, 1
mov rdi, 1
lea rsi, msg 
mov rdx, MSG_LEN
syscall
ret