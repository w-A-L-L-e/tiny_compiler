format ELF executable 3
entry start

segment readable writable
x dd 0

segment readable executable
start:
    ; Set up the stack frame
    push ebp         ; Save the old base pointer
    mov ebp, esp     ; Set the new base pointer
    mov eax, 42
    mov [ebp-4], eax
    mov eax, [ebp-4]
    cmp eax, 0
    je else_0
    mov eax, 22
    mov ebx, eax
    jmp program_end
else_0:
program_end:
    mov eax, 1
    int 0x80 
