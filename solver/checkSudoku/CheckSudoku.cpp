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