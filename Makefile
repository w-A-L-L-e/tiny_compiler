all: compiler

compiler: lexer.o parser.o compiler.o main.o

main.o: main.c

compiler.o: compiler.c compiler.h

parser.o: parser.c parser.h

lexer.o: lexer.h lexer.c

run: compiler
	clear
	./compiler
	cat output.asm
	fasm output.asm
	chmod 755 output
	./output


clean:
	@rm -vf *.o compiler output

