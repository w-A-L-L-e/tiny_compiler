
format ELF executable 3
entry start

segment readable writable
x dd 0

segment readable executable
start:
    push ebp
    mov ebp, esp
    mov eax, 42
    mov [x], eax
    mov eax, [x]
    cmp eax, 0
    je else_0
    mov eax, [x]
    mov ebx, eax
    jmp program_end
else_0:
program_end:
    mov eax, 1
    int 0x80
