;Adding two numbers from the user and printing the result
;Note that it adds two single digit numbers which will result in single digit number..
;ok now it will be successful since the input will be take in the char type array we need our data first int to integer..
section .data
msg: db "The Result: "
n1: db "Enter number 1: "
n2: db "Enter number 2: "
section .bss
v1: RESB 2
v2: RESB 2
res: RESB 1
section .text
global _start
_start:
;Asking for the first number from the user
mov rax, 1
mov rdi, 1
mov rsi, n1
mov rdx, 16
syscall
;Getting the first number from the user
mov rax, 0
mov rdi, 0
mov rsi, v1
mov rdx, 2
syscall
;Asking for the Second number from the user
mov rax, 1
mov rdi, 1
mov rsi, n2
mov rdx, 16
syscall
;Getting the Second Number
mov rax, 0
mov rdi, 0
mov rsi, v2
mov rdx, 2
syscall
;Performing the add operation
mov al, byte [v1]
sub al, 48
mov bl, byte [v2]
sub bl, 48
add al, bl
add al, 48
mov byte[res], al
;printing the result
mov rax, 1
mov rdi, 1
lea rsi, res
mov rdx, 1
syscall
;exiting the program
mov rax, 60
mov rdi, 54
syscall