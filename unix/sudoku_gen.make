SUDOKU_GEN_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku_gen.exe: sudoku_gen.o
	gcc -o sudoku_gen.exe sudoku_gen.o

sudoku_gen.o: ../src/sudoku_gen.c sudoku_gen.make
	gcc ${SUDOKU_GEN_C_FLAGS} -o sudoku_gen.o ../src/sudoku_gen.c
