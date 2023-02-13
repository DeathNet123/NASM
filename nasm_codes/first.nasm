;This is the first code in nasm...
section .data
msg db "Hello world!", 0xA; 0xA is new line character...
EXIT_STATUS equ 54; my exist status...
section .bss
;going to be empty
section .text ; start of the code
global _start
_start:
;int write(int fd, void *buf, int count);

mov eax, -1
movsx rax, eax
mov rax, 1 ;ID for the write system call..
mov rdi, 1 ;file descriptor for the write system call one is moved because stdout is 1
mov rsi, msg ;buffer from which the write system call will read and write 
mov rdx, 13 ;Number of characters + one new line character in the end..
syscall ;calling the write system call once all the respective registers are set..
mov rax, 60 ;Id for the exist system
mov rdi, EXIT_STATUS
syscall ;calling the exit system call...