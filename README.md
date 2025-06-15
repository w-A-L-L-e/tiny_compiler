# tiny_compiler

So tried to use chat gpt to see if it can create some reasonable compiler code.
Conclusion it did a pretty good job on the lexer. The recursive descent parser did however (and still does)
contain some flaws. And with the compiling to fasm part it took many iterations to get something that kinda worked
but segfaulted when running. Then a few hours of debugging and actually doing some manual programming later we have a
minimal compiler from a script into fasm assembly.

## compiling the tiny compiler

```
make
cc    -c -o compiler.o compiler.c
cc    -c -o lexer.o lexer.c
cc    -c -o parser.o parser.c
cc    -c -o main.o main.c
cc   compiler.o lexer.o parser.o main.o   -o compiler
```

## run compiler, generate assembly and compile it with fasm

```
sudo apt get install fasm
make run
```

Make run shows the generated assembly output for the script (currently that is just hardcoded, but
future version should read the input script also from a file ofcourse):

Script code in main.c that we now want to compile:

```
    const char *code = "int x; x = 42; if (x) { return 22; }";
```

Compile it to assembly then run fasm to compile actual executable:

```
make run
```

And after this it uses fasm to compile and run it. Which will exit the program with an error
code 22 for demo purposes.

```
./compiler
Assembly written to output.asm
cat output.asm
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
fasm output.asm
flat assembler  version 1.73.32  (16384 kilobytes memory)
3 passes, 155 bytes.
chmod 755 output

./output
make: *** [Makefile:19: run] Error 22
```

This is cool and all but its far from turing complete. And it seems whenever you try to add more complexity here it
just gets worse and worse even with the latest chat gpt. Whenever I tried to fix bugs or add new features (like else part of if / else statements)
it broke other parts and started heavily hallucinating solutions.

So it's cool that chat gpt can generate like 90% good code which saves a lot of typing and looking up trivial stuff with google.
But on the other hand I wrote a far more complete and
fully turing complete functional version of my own decades ago https://github.com/w-A-L-L-e/wsbasic.
Here I did have some background but the actual initial implemantation of wsbasic took me only 1 or 2 weekends. Ofcourse
I did have about 4 years experience writing parsers for my PhD back then.

I'm pretty sure I can now make the C version of such a recursive descent parser + compiler faster using chat gpt. However
It's also pretty clear someone without inital compiler knowledge will never get this working by just prompting and 'vibe coding' it.

Which might be good news I guesse. It's gonna be a while before real programmers are obsolete ;)
