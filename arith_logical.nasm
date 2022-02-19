section .data
msg: db "Hello World this is Arslan!", 0xA
MSG_LEN equ $ - msg ; msg length
EXIT_STATUS equ 54 ;This is convention i guess otherwise would be something else ...
section .text
global _start ; this is again the convention would have used the main instead would be no difference
_start:
;printing the msg on screen
mov rax, 1
mov rdi, 1
lea rsi, msg
mov rdx, MSG_LEN
syscall
;end of the syscall for the write function.
mov rax, 16d
mov rbx, 16d
add rax, rbx
sub rax, rbx
mul rbx
div rbx

;exiting the program
mov rax, 60
mov rdi, EXIT_STATUS
syscall