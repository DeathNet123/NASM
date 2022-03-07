SECTION .data 
formatStr1 db "What is your name?", 0
formatStr2 db "Hello Mr. %s", 0xA
SECTION .bss
name resb 10
SECTION .text 
global main
extern puts
extern gets
extern exit
main:
;prompt the user to input his/her name
lea rdi, formatStr1
call puts
mov rdi, 0 
call exit