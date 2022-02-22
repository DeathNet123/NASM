;this program is the new version of the add program written before which was able to add only 3-bit numbers whose result should also be in 3-bits 
;Now it is is time to make some more powerfull adder..
section .data
msg: db "Welcome to the add program", 0xA
MSG_LEN equ $ - msg
n1: db "Enter number 1: "
n2: db "Enter number 2: "
EXIT_STATUS equ 54
section .bss
v1 RESB 9
v2 RESB 9
res RESB 8
section .text
global _start
_start:
;Printing the hello msg
mov rax, 1
mov rdi, 1
mov rsi, msg
mov rdx, MSG_LEN
syscall
;Asking for the first number from the user
mov rax, 1
mov rdi, 1
mov rsi, n1
mov rdx, 16
syscall
;Getting the first Number
mov rax, 0
mov rdi, 0
mov rsi, v1
mov rdx, 8
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
mov rdx, 8
syscall
;calculating the length of the v1
mov rcx, v1
call _strlen
push rax ;saving the result on stack
;calculating the length of the v2
mov rcx, v2
call _strlen
push rax ;saving the result on stack
;converting the v2 from string to integer
pop rcx
lea rdx, v2
call _stoi
mov r15, rax
;converting the v2 from string to integer
pop rcx
lea rdx, v1
call _stoi
mov r12, rax
;doing the addition
add r12, r15
push r12 ;putting result on stack for debugging purpose
_end:
mov rax, 60
pop rdi
syscall
_strlen:
mov rax, -1
_repeat:
add rax, 1
cmp byte[rcx + rax], 0xA
jg _repeat
ret
_stoi:
mov rax, 0 ;intializing the result
mov r11, 1
sub rcx, 1
_loop:
mov r8b, byte[rdx + rcx]
sub r8b, 48
iMUL r8, r11
add rax, r8
imul r11, 10d
sub rcx, 1
cmp rcx, 0
jge _loop
ret