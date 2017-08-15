#include <stdio.h>
#include "MySudokuMain.h"

int main(int argc, char *argv[]){
	Sudoku *sudoku = ReadSudoku();

	printf("Unsolved Sudoku:\n");
	PrintSudoku(sudoku);

	SolveSudoku(sudoku);

	printf("\n\nSolved Sudoku:\n");
	PrintSudoku(sudoku);

	FreeSudoku(sudoku);

	return 0;
}