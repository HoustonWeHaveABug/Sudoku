SUDOKU_GEN_DEBUG_C_FLAGS=-c -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku_gen_debug.exe: sudoku_gen_debug.o
	gcc -g -o sudoku_gen_debug.exe sudoku_gen_debug.o

sudoku_gen_debug.o: ../src/sudoku_gen.c sudoku_gen_debug.make
	gcc ${SUDOKU_GEN_DEBUG_C_FLAGS} -o sudoku_gen_debug.o ../src/sudoku_gen.c
