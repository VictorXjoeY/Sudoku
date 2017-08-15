#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MySudokuMain.h"
#include "MySudokuUtils.h"
#include "MySudokuStrategies.h"

bool SymbolInLine(Sudoku *sudoku, int x, int v){
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[x][i]->value == sudoku->symbol[v]){
			return true;
		}
	}

	return false;
}

bool SymbolInColumn(Sudoku *sudoku, int y, int v){
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[i][y]->value == sudoku->symbol[v]){
			return true;
		}
	}

	return false;
}

bool SymbolInRegion(Sudoku *sudoku, int r, int v){
	int line = r / sudoku->region_dimension;
	int column = r % sudoku->region_dimension;
	int i, j;

	for (i = line * sudoku->region_dimension; i < line * sudoku->region_dimension + sudoku->region_dimension; i++){
		for (j = column * sudoku->region_dimension; j < column * sudoku->region_dimension + sudoku->region_dimension; j++){
			if (sudoku->cell[i][j]->value == sudoku->symbol[v]){
				return true;
			}
		}
	}

	return false;
}

int GetRegion(Sudoku *sudoku, int x, int y){
	int line = x / sudoku->region_dimension;
	int column = y / sudoku->region_dimension;

	return sudoku->region_dimension * line + column;
}

void GenerateCellsPossibilities(Sudoku *sudoku){
	int i, j, k;

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			for (k = 0; k < sudoku->dimension; k++){
				if (sudoku->cell[i][j]->value == '.' and sudoku->line[i]->possibility[k] and sudoku->column[j]->possibility[k] and sudoku->region[GetRegion(sudoku, i, j)]->possibility[k]){
					sudoku->cell[i][j]->possibility[k] = true;
					sudoku->cell[i][j]->n++;
				}
			}
		}
	}
}

void GenerateGroupsPossibilities(Sudoku *sudoku){
	int i, j;

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			if (!SymbolInLine(sudoku, i, j)){
				sudoku->line[i]->possibility[j] = true;
				sudoku->line[i]->n++;
			}

			if (!SymbolInColumn(sudoku, i, j)){
				sudoku->column[i]->possibility[j] = true;
				sudoku->column[i]->n++;
			}

			if (!SymbolInRegion(sudoku, i, j)){
				sudoku->region[i]->possibility[j] = true;
				sudoku->region[i]->n++;
			}
		}
	}
}

void GeneratePossibilities(Sudoku *sudoku){
	GenerateGroupsPossibilities(sudoku);
	GenerateCellsPossibilities(sudoku);
}

void LineUpdatePossibilities(Sudoku *sudoku, int x, int v){
	int i;

	sudoku->line[x]->possibility[v] = false;
	sudoku->line[x]->n--;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[x][i]->n and sudoku->cell[x][i]->possibility[v]){
			sudoku->cell[x][i]->n--;
			sudoku->cell[x][i]->possibility[v] = false;
		}
	}
}

void ColumnUpdatePossibilities(Sudoku *sudoku, int y, int v){
	int i;

	sudoku->column[y]->possibility[v] = false;
	sudoku->column[y]->n--;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[i][y]->n and sudoku->cell[i][y]->possibility[v]){
			sudoku->cell[i][y]->n--;
			sudoku->cell[i][y]->possibility[v] = false;
		}
	}
}

void RegionUpdatePossibilities(Sudoku *sudoku, int r, int v){
	int line = r / sudoku->region_dimension;
	int column = r % sudoku->region_dimension;
	int i, j;

	sudoku->region[r]->possibility[v] = false;
	sudoku->region[r]->n--;

	for (i = line * sudoku->region_dimension; i < line * sudoku->region_dimension + sudoku->region_dimension; i++){
		for (j = column * sudoku->region_dimension; j < column * sudoku->region_dimension + sudoku->region_dimension; j++){
			if (sudoku->cell[i][j]->possibility[v]){
				sudoku->cell[i][j]->possibility[v] = false;
				sudoku->cell[i][j]->n--;
			}
		}
	}
}

void UpdatePossibilities(Sudoku *sudoku, int x, int y, int v){
	LineUpdatePossibilities(sudoku, x, v);
	ColumnUpdatePossibilities(sudoku, y, v);
	RegionUpdatePossibilities(sudoku, GetRegion(sudoku, x, y), v);
}

void FillCell(Sudoku *sudoku, int x, int y, int v){
	sudoku->cell[x][y]->n = 0;
	memset(sudoku->cell[x][y]->possibility, false, sudoku->dimension * sizeof(bool));
	sudoku->cell[x][y]->value = sudoku->symbol[v];

	UpdatePossibilities(sudoku, x, y, v);
}

void PrintPossibilities(Sudoku *sudoku){
	int i, j, k;

	printf("Possibilities:\n");

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			if (sudoku->cell[i][j]->n){
				printf("(%d, %d):", i, j);

				for (k = 0; k < sudoku->dimension; k++){
					if (sudoku->cell[i][j]->possibility[k]){
						printf(" %c", sudoku->symbol[k]);
					}
				}

				printf("\n");
			}
		}
	}

	printf("\n");
}

bool SolvedSudoku(Sudoku *sudoku){
	int i, j;

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			if (sudoku->cell[i][j]->value == '.'){
				return false;
			}
		}
	}

	return true;
}