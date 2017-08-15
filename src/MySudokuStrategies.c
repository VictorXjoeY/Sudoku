#include <stdio.h> // Not necessary

#include "MySudokuMain.h"
#include "MySudokuUtils.h"
#include "MySudokuStrategies.h"

void SinglePossibilityRule(Sudoku *sudoku){
	int i, j, k;

	for (i = 0; i < sudoku->dimension; i++){
		for (j = 0; j < sudoku->dimension; j++){
			if (sudoku->cell[i][j]->n == 1){
				k = 0;

				while (!sudoku->cell[i][j]->possibility[k]){
					k++;
				}

				FillCell(sudoku, i, j, k);
			}
		}
	}
}

void LineOnlySquareRule(Sudoku *sudoku, int x, int v){
	int counter = 0;
	int y, i;

	for (i = 0; i < sudoku->dimension and counter <= 1; i++){
		if (sudoku->cell[x][i]->possibility[v]){
			counter++;
			y = i;
		}
	}

	if (counter == 1){
		FillCell(sudoku, x, y, v);
	}
}

void ColumnOnlySquareRule(Sudoku *sudoku, int y, int v){
	int counter = 0;
	int x, i;

	for (i = 0; i < sudoku->dimension and counter <= 1; i++){
		if (sudoku->cell[i][y]->possibility[v]){
			counter++;
			x = i;
		}
	}

	if (counter == 1){
		FillCell(sudoku, x, y, v);
	}
}

void RegionOnlySquareRule(Sudoku *sudoku, int r, int v){
	int line = r / sudoku->region_dimension;
	int column = r % sudoku->region_dimension;
	int counter = 0;
	int x, y, i, j;

	for (i = line * sudoku->region_dimension; i < line * sudoku->region_dimension + sudoku->region_dimension and counter <= 1; i++){
		for (j = column * sudoku->region_dimension; j < column * sudoku->region_dimension + sudoku->region_dimension and counter <= 1; j++){
			if (sudoku->cell[i][j]->possibility[v]){
				counter++;
				x = i;
				y = j;
			}
		}
	}

	if (counter == 1){
		FillCell(sudoku, x, y, v);
	}
}

void OnlySquareRule(Sudoku *sudoku){
	int i, j;

	// Iterating over Lines, Columns and Regions.
	for (i = 0; i < sudoku->dimension; i++){

		// Iterating over Symbols.
		for (j = 0; j < sudoku->dimension; j++){
			if (sudoku->line[i]->possibility[j]){
				LineOnlySquareRule(sudoku, i, j);
			}

			if (sudoku->column[i]->possibility[j]){
				ColumnOnlySquareRule(sudoku, i, j);
			}

			if (sudoku->region[i]->possibility[j]){
				RegionOnlySquareRule(sudoku, i, j);
			}
		}
	}
}

void UpdateLineSubgroupPossibilities(Sudoku *sudoku, int x, int v){
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[x][i]->possibility[v]){
			return;
		}
	}

	sudoku->line[x]->possibility[v] = false;
	sudoku->line[x]->n--;
}

void UpdateColumnSubgroupPossibilities(Sudoku *sudoku, int y, int v){
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[i][y]->possibility[v]){
			return;
		}
	}

	sudoku->column[y]->possibility[v] = false;
	sudoku->column[y]->n--;
}

void RegionUpdatePossibilitiesExcludingLine(Sudoku *sudoku, int r, int v, int x){
	int line = r / sudoku->region_dimension;
	int column = r % sudoku->region_dimension;
	int i, j;

	for (i = line * sudoku->region_dimension; i < line * sudoku->region_dimension + sudoku->region_dimension; i++){
		for (j = column * sudoku->region_dimension; j < column * sudoku->region_dimension + sudoku->region_dimension; j++){
			if (i != x and sudoku->cell[i][j]->possibility[v]){
				printf("Deu bom! Tirando a possibilidade %c da casa (%d, %d)\n", sudoku->symbol[v], i, j);
				sudoku->cell[i][j]->possibility[v] = false;
				sudoku->cell[i][j]->n--;
				UpdateLineSubgroupPossibilities(sudoku, i, v);
				UpdateColumnSubgroupPossibilities(sudoku, j, v);
			}
		}
	}
}

void RegionUpdatePossibilitiesExcludingColumn(Sudoku *sudoku, int r, int v, int y){
	int line = r / sudoku->region_dimension;
	int column = r % sudoku->region_dimension;
	int i, j;

	for (i = line * sudoku->region_dimension; i < line * sudoku->region_dimension + sudoku->region_dimension; i++){
		for (j = column * sudoku->region_dimension; j < column * sudoku->region_dimension + sudoku->region_dimension; j++){
			if (j != y and sudoku->cell[i][j]->possibility[v]){
				printf("Deu bom! Tirando a possibilidade %c da casa (%d, %d)\n", sudoku->symbol[v], i, j);
				sudoku->cell[i][j]->possibility[v] = false;
				sudoku->cell[i][j]->n--;
				UpdateLineSubgroupPossibilities(sudoku, i, v);
				UpdateColumnSubgroupPossibilities(sudoku, j, v);
			}
		}
	}
}

void LineSubgroupExclusionRule(Sudoku *sudoku, int x, int v){
	int r = -1;
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[x][i]->possibility[v]){
			if (r == -1){
				r = GetRegion(sudoku, x, i);
			}
			else if (r != GetRegion(sudoku, x, i)){
				return;
			}
		}
	}

	printf("Aplicando nova regra para o simbolo %c na regiao %d, linha %d\n", sudoku->symbol[v], r, x);

	RegionUpdatePossibilitiesExcludingLine(sudoku, r, v, x);
}

void ColumnSubgroupExclusionRule(Sudoku *sudoku, int y, int v){
	int r = -1;
	int i;

	for (i = 0; i < sudoku->dimension; i++){
		if (sudoku->cell[i][y]->possibility[v]){
			if (r == -1){
				r = GetRegion(sudoku, i, y);
			}
			else if (r != GetRegion(sudoku, i, y)){
				return;
			}
		}
	}

	printf("Aplicando nova regra para o simbolo %c na regiao %d, coluna %d\n", sudoku->symbol[v], r, y);

	RegionUpdatePossibilitiesExcludingColumn(sudoku, r, v, y);
}

void SubgroupExclusionRule(Sudoku *sudoku){
	int i, j;

	// Iterating over Lines and Columns.
	for (i = 0; i < sudoku->dimension; i++){

		// Iterating over Symbols.
		for (j = 0; j < sudoku->dimension; j++){
			if (sudoku->line[i]->possibility[j]){
				LineSubgroupExclusionRule(sudoku, i, j);
			}

			if (sudoku->column[i]->possibility[j]){
				ColumnSubgroupExclusionRule(sudoku, i, j);
			}
		}
	}
}