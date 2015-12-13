SUDOKU_DEBUG_C_FLAGS=-c -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku_debug.exe: sudoku_debug.o
	gcc -g -o sudoku_debug.exe sudoku_debug.o

sudoku_debug.o: ../src/sudoku.c sudoku_debug.make
	gcc ${SUDOKU_DEBUG_C_FLAGS} -o sudoku_debug.o ../src/sudoku.c
