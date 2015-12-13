SUDOKU_DLX_DEBUG_C_FLAGS=-c -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

sudoku_dlx_debug.exe: sudoku_dlx_debug.o
	gcc -g -o sudoku_dlx_debug.exe sudoku_dlx_debug.o

sudoku_dlx_debug.o: ../src/sudoku_dlx.c sudoku_dlx_debug.make
	gcc ${SUDOKU_DLX_DEBUG_C_FLAGS} -o sudoku_dlx_debug.o ../src/sudoku_dlx.c
