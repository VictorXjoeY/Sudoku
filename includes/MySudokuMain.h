#ifndef MY_SUDOKU_MAIN_H
#define MY_SUDOKU_MAIN_H

#define true 1
#define false 0
#define and &&
#define or ||

typedef unsigned char bool;

typedef struct{
	bool *possibility;
	char value;
	int n;
}Cell;

typedef struct{
	bool *possibility;
	int n;
}Group;

typedef struct{
	Cell ***cell;
	Group **line;
	Group **column;
	Group **region;
	char *symbol;
	int dimension;
	int region_dimension;
}Sudoku;

Sudoku *ReadSudoku();

void SolveSudoku(Sudoku *);

void PrintSudoku(const Sudoku *);

void FreeSudoku(Sudoku *);

#endif