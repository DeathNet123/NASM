;taking user input try one...
section .data
EXIT_STATUS equ 54
msg db "You printed", 0xA
section .bss
input RESB 13
section .text
global _start
_start:
mov rax, 0 ;read system call id..
mov rdi, 0 ;fd set to 0 as stdin..
mov rsi, input ;set the buffer to input second argument of read..
mov rdx, 13 ;size..
syscall
mov rax, 1 ;write syscall id..
mov rdi, 1 ;fd set to stdout..
mov rsi, msg ;set the buffer to msg second argument of write..
mov rdx, 12
syscall
mov rax, 1 ;write syscall id..
mov rdi, 1 ;fd set to stdout..
mov rsi, input ;set the buffer to msg second argument of write..
mov rdx, 13
syscall
;exiting the program..
mov rax, 60 ;exit system call id..
mov rdi, EXIT_STATUS
syscall
