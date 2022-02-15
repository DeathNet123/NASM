;Adding two numbers from the user and printing the result
;Note that it adds two 16-bits number
section .data
msg db "The Result: "
n1 db "Enter number 1: "
n2 db "Enter number 2: "
section .bss
v1 RESW 1
v2 RESW 1
section .text
global _start
_start:
;Asking for the first number from the user
mov rax, 1
mov rdi, 1
mov rsi, n1
mov rdx, 16
syscall
;Taking the first input from the user
mov rax, 0
mov rdi, 0
mov rsi, v1
mov rdx, 16
syscall
;Asking for the second number
mov rax, 1
mov rdi, 1
mov rsi, n2
mov rdx, 16
syscall
;Taking the second input from the user...
mov rax, 0
mov rdi, 0
mov rsi, v2
mov rdx, 16
syscall
;Printing the result string..
mov rax, 1
mov rdi, 1
mov rsi, msg
mov rdx, 12
syscall
;Adding two numbers
mov r11, v2
Add r11, v1
;Printing the acutal result..
mov rax, 1
mov rdi, 1
mov rsi, r11
mov rdx, 64
syscall
;exiting the program
mov rax, 60
mov rdi, 54
syscall