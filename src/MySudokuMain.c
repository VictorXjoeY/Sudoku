#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MySudokuMain.h"
#include "MySudokuUtils.h"
#include "MySudokuStrategies.h"

int IntSqrt(int x){
	return (int)sqrt(x + 1e-7);
}

Sudoku *ReadSudoku(){
	Sudoku *sudoku = (Sudoku *)malloc(sizeof(Sudoku));
	int i, j;

	// Reading Sudoku's dimensions.
	scanf("%d%*c", &(sudoku->dimension));

	sudoku->symbol = (char *)malloc(sudoku->dimension * sizeof(char));
	sudoku->cell = (Cell ***)malloc(sudoku->dimension * sizeof(Cell **));
	sudoku->line = (Group **)malloc(sudoku->dimension * sizeof(Group *));
	sudoku->column = (Group **)malloc(sudoku->dimension * sizeof(Group *));
	sudoku->region = (Group **)malloc(sudoku->dimension * sizeof(Group *));

	// Setting Sudoku's regions dimensions.
	sudoku->region_dimension = IntSqrt(sudoku->dimension);

	// Reading used symbols.
	for (i = 0; i < sudoku->dimension; i++){
		scanf("%c%*c", sudoku->symbol + i);
	}

	scanf("%*c");

	// Reading Sudoku.
	for (i = 0; i < sudoku->dimension; i++){
		sudoku->cell[i] = (Cell **)malloc(sudoku->dimension * sizeof(Cell *));

		if (i % sudoku->region_dimension == 0 and i != 0){
			scanf("%*s%*c");
		}

		for (j = 0; j < sudoku->dimension; j++){
			if (j % sudoku->region_dimension == 0 and j != 0){
				scanf("%*c%*c");
			}

			sudoku->cell[i][j] = (Cell *)malloc(sizeof(Cell));

			sudoku->cell[i][j]->possibility = (bool *)calloc(sudoku->dimension, sizeof(bool));

			sudoku->cell[i][j]->n = 0;

			scanf("%c%*c", &(sudoku->cell[i][j]->value));
		}

		sudoku->line[i] = (Group *)malloc(sizeof(Group));
		sudoku->column[i] = (Group *)malloc(sizeof(Group));
		sudoku->region[i] = (Group *)malloc(sizeof(Group));

		sudoku->line[i]->possibility = (bool *)calloc(sudoku->dimension, sizeof(bool));
		sudoku->column[i]->possibility = (bool *)calloc(sudoku->dimension, sizeof(bool));
		sudoku->region[i]->possibility = (bool *)calloc(sudoku->dimension, sizeof(bool));

		sudoku->line[i]->n = 0;
		sudoku->column[i]->n = 0;
		sudoku->region[i]->n = 0;
	}

	return sudoku;
}

void SolveSudoku(Sudoku *sudoku){
	int counter = 0;

	GeneratePossibilities(sudoku);
	// PrintPossibilities(sudoku);

	while (!SolvedSudoku(sudoku) and counter < 5){
		SinglePossibilityRule(sudoku);
		OnlySquareRule(sudoku);
		// SubgroupExclusionRule(sudoku);
		// PrintPossibilities(sudoku);
		counter++;
	}
}

void PrintSudoku(const Sudoku *sudoku){
	int i, j;

	for (i = 0; i < sudoku->dimension; i++){
		if (i % sudoku->region_dimension == 0 and i != 0){
			for (j = 0; j < 2 * sudoku->dimension + 2 * (sudoku->region_dimension - 1) - 1; j++){
				printf("-");
			}
			printf("\n");
		}

		for (j = 0; j < sudoku->dimension; j++){
			if (j % sudoku->region_dimension == 0 and j != 0){
				printf("| ");
			}

			printf("%c ", sudoku->cell[i][j]->value);
		}

		printf("\n");
	}
}

void FreeSudoku(Sudoku *sudoku){
	int i, j;

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			free(sudoku->cell[i][j]->possibility);
			free(sudoku->cell[i][j]);
		}

		free(sudoku->cell[i]);

		free(sudoku->line[i]->possibility);
		free(sudoku->column[i]->possibility);
		free(sudoku->region[i]->possibility);

		free(sudoku->line[i]);
		free(sudoku->column[i]);
		free(sudoku->region[i]);
	}

	free(sudoku->line);
	free(sudoku->column);
	free(sudoku->region);

	free(sudoku->cell);
	free(sudoku->symbol);
	free(sudoku);
}