#include "CheckSudoku.h"
#include "utility.h"

bool checkSudoku(int puzzle[9][9]) {
	//check if the sudoku is solveable, and has only a single solution
	//(a sudoku with more than one solution is not valid)
	int solutions = 0;
	solveRecursive(puzzle, 0, 0, solutions);
	return (solutions == 1);
}

void solveRecursive(int puzzle[9][9], int rowNum, int colNum, int& solutions) {
	int nextRow = (colNum == 8) ? rowNum + 1 : rowNum;
	int nextCol = (colNum == 8) ? 0 : colNum + 1;

	//if puzzle has more than one solution, it isn't valid and there's no reason to keep executing
	if (solutions > 1) {
		return;
	}

	//check if the puzzle is solved, increment solutions and return if so
	if (isSolved(puzzle)) {
		solutions++;
		return;
	}

	//if this space is one of the ones given by the puzzle, don't change it
	if (puzzle[rowNum][colNum] != 0) {
		solveRecursive(puzzle, nextRow, nextCol, solutions);
		return;
	}

	//try different values in the current square, recursing after each one
	for (int i = 1; i <= 9; i++) {
		puzzle[rowNum][colNum] = i;
		if (isValid(puzzle)) {
			solveRecursive(puzzle, nextRow, nextCol, solutions);
		}
		if (solutions > 1) {
			return;
		}
	}

	puzzle[rowNum][colNum] = 0;
}

bool duplicates(int array[9])
{
	//ensure there are no non-zero duplicates in a given array
	for (int i = 0; i < 8; i++) {
		for (int j = 8; j > i; j--) {
			if ((array[i] != 0) && (array[j] != 0) && (array[i] == array[j]))
				return true;
		}
	}
	return false;
}

bool isValid(int puzzle[9][9])
{
	//return true if the puzzle has no duplicates in its rows/columns/boxes
	//used by the recursive checker
	int checkArray[9];

	//Check all rows to see if they have duplicates.
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			checkArray[j] = puzzle[i][j];
		}
		if (duplicates(checkArray)) {
			return false;
		}
	}

	//Check all columns to see if they have duplicates.
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			checkArray[j] = puzzle[j][i];
		}
		if (duplicates(checkArray)) {
			return false;
		}
	}

	//Check all boxes to see if they have duplicates.
	for (int i = 0; i <= 6; i = i + 3) {
		for (int j = 0; j <= 6; j = j + 3) {
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					checkArray[(k * 3) + l] = puzzle[k + i][l + j];
				}
			}
			if (duplicates(checkArray)) {
				return false;
			}
		}
	}

	return true;
}

int nextRow(int row_num, int column_num)
{
	//Row should increase only if the current function call is on the last column.
	if (column_num == 8) {
		row_num++;
		return row_num;
	}
	return row_num;
}

int nextColumn(int row_num, int column_num)
{
	//If the current function call is on the last column, reset to the first column (of the next row).
	//Increment the column in every other case to continue traversing a row.
	if (column_num == 8) {
		return  0;
	}
	else {
		column_num++;
		return column_num;
	}
}