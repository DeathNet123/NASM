section .data
arr db 0x65, 0x2a, 0x1c, 0x46 ; this is equal to 32 bits * 4
section .text
global _start
_start:
mov eax, dword[arr] ; double word equals to 32
mov rbx, arr ;moved completely 
mov eax, dword[arr] 
mov eax, dword[arr+4] ; have to ask this from sir...
mov rax, [arr]
;exiting the program
mov rax, 60
mov rdi, 54
syscall