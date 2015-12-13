SUDOKU_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku.exe: sudoku.o
	gcc -o sudoku.exe sudoku.o

sudoku.o: ../src/sudoku.c sudoku.make
	gcc ${SUDOKU_C_FLAGS} -o sudoku.o ../src/sudoku.c
