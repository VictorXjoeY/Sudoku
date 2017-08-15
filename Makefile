ifdef Sudoku
	in = < Sudokus/$(Sudoku).in
	out = > Sudokus/$(Sudoku).out
endif

all:
	gcc -o main src/*.c -I./includes -lm
debug:
	gcc -o main src/*.c -I./includes -lm -g -Wall
run:
	./main $(in)
out:
	./main $(in) $(out)
fullrun:
	valgrind -v --track-origins=yes --leak-check=full ./main $(in)