# tiny_compiler proof of concept compiler generated with AI

So tried to use chat gpt to see if it can create some reasonable compiler code.
While it did a fairly good job. And I was impressed at first glance. The further
I looked at what it actually created I noticed we are still a really long way from replacing
all human programmers.

Basically I prompted it to make a small subset of a scripting language by writing a minimal
lexer than a minimal recursive descent parser for it and then a compiler to create fasm assembly that
could be compiled into an actual exe.

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
code 22 for demo purposes (I had to fix the compilers code generation manually here as the original
versions generated either just segfaulted or gave fasm compile errors).
Even after the bugfixes the generated fasm is far from optimal and contains flaws + memory leaks:

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

I'm pretty sure I can now make the C version of such a recursive descent parser + compiler faster using chat gpt
than I would have been able to without it (also back then I used c++ and vector of TreeNode which solved the whole
limitation of malloc's not freed and resizing nodes in TreeNode aka ASTNode in this codebase done properly). However
it's also pretty clear someone without actual compiler design knowledge will never get this fully
working by just prompting and 'vibe coding' it.

Which might be good news I guesse. It's gonna be a while before real programmers are obsolete ;)

## Flaws and issues with current generated code

Just to make it verry clear. Yes using latest gpt on 16/6/2025 did save me a lot of typing.
However the more I look at the generated code its obvious this is nowhere near to getting it production ready.
There are a lot of hidden subtle bugs and memory leaks that most likely any vibe coder
wouldn't notice until it totally crashes on them.

Current issues I just saw at first glance and was unsuccesful to let chatgpt fix using
some more prompting:

- In parser.c multiple malloc's are made but nothing is freed (Big memory leak)

- https://github.com/w-A-L-L-e/tiny_compiler/blob/b7c1f9b54ad94fdaa5a954431eb0b2f27bdc5f5f/parser.c#L94
  : on this line we have the body of an if statgement. Issue 1: its just hardcoded
  to 100 nodes (meaning anything more than 100 statements will result in the compiler crashing with segfault).
  Again this malloc is also never freed

- If statements, the else part is totally not implemented however its hinted to be
  implemented in the generated compiler code later on.

- Expressions: only assignments to constants and variables are done.
  The interesting part to do actual arithmetic expressions is omitted entirely, agaom prompting GPT here
  only broke more than it fixed.

- The original recursive print_ast as was generated by chat gpt didn't work at all. I had to manually read and
  tweak it to actually print the body of the program. (Again prompting more just would make
  it all worse and break more than just reading the implemantation and manually fixing it). https://github.com/w-A-L-L-e/tiny_compiler/blob/b7c1f9b54ad94fdaa5a954431eb0b2f27bdc5f5f/parser.c#L146

- In the compiler we just get the bare minimum generated that just barely satisfies
  the hardcoded script in main.c that has just 1 assignment and 1 if without else and a return statement
  (and even then the generated version had a lot of bugs that needed manual fixing).

## Conclusion for me in 2025:

AI code generation got better but it's still a decade or more IMHO
before we can truly replace some programmers entirely (and more likely even longer away before it can truly replace
a senior programmer).
It's nowhere near as good or dangerous as the current AI hype and marketing on youtube is wanting
to make it look to sell more subscriptions.

Sure it can refactor and save you some typing for trivial cases. But it still lacks the deeper understanding
and reasoning that humans have when writing and improving code. Even if you get to market there's going
to be issues that in order to fix you'll either need to hire someone to read/review your entire codebase and then
fix + continue all the work done. Most likely this will only happen after months of prompting and GPT like subscriptions
being paid only to realise you probable would have gotten to the finish just as fast when hiring a proper developer from the get go.

It's a bit like the no-code hypes we already had 20 years ago like dreamweaver and mulesoft. It gets you up
and running to 80% quickly but after that you better understand everything it generated to be able to get to the
finish line. What's better this time around is you'll end up with some code you can further improve and
extend yourself. In the past with the other no-code solutions most of the time the generated code was either all locked
down/hidden or totally ugly code you wouldn't want to continue working in.

So in that sence it does indeed improve our productivity. As I do see that the generated code in this
repo is not terrible. It needs a lot of fixes but also the stuff that is ok/trivial is pretty cleanly executed.
But as far as I see it we'll need proper computer scientists for many years to come to actually keep everything working properly...
