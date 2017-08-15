#ifndef MY_SUDOKU_UTILS_H
#define MY_SUDOKU_UTILS_H

void GeneratePossibilities(Sudoku *sudoku);

int GetRegion(Sudoku *, int, int);

void FillCell(Sudoku *, int, int, int);

void PrintPossibilities(Sudoku *sudoku);

bool SolvedSudoku(Sudoku *);

#endif