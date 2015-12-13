SUDOKU_DLX_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku_dlx.exe: sudoku_dlx.o
	gcc -o sudoku_dlx.exe sudoku_dlx.o

sudoku_dlx.o: ../src/sudoku_dlx.c sudoku_dlx.make
	gcc ${SUDOKU_DLX_C_FLAGS} -o sudoku_dlx.o ../src/sudoku_dlx.c
