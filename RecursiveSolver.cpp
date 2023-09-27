#include "Utility.h"
#include "RecursiveSolver.h"

/*
	function description:
	Utilizes a recursive algorithm to be able to solve any sudoku puzzle.

	PARAM: 9x9 integer array puzzle[9][9]: array representing a sudoku puzzle.
	PARAM: integer row_num, index representing the row of the array index to be filled (0 for first call).
	PARAM: integer column_num, index representing the column of the array index to be filled (0 for first call).
	RETURN: boolean variable, tells user whether the sudoku puzzle was solved or not.
*/
bool solveSudoku(int puzzle[9][9], int row_num, int column_num)
{
	// Initialize variables and determine the indices to be called in the next recursive call.
	int next_row, next_column;
	next_row = nextRow(row_num, column_num);
	next_column = nextColumn(row_num, column_num);

	//Base case - if the sudoku is solved, return true back through every call.
	if (isSolved(puzzle))
		return true;

	//If the current call of the function lands on an already filled space, 
	//'skip' it by calling the next index without editing the sudoku array.
	if (puzzle[row_num][column_num] != 0)
		return solveSudoku(puzzle, next_row, next_column);

	//If the sudoku is not solved, and the current call isn't on a filled space it is time to 'guess and check'.
	//
	//The guess: looping through the numbers 1-9.
	//
	//First check: checking whether the guess creates a sudoku that is still valid.
	//If it does not create a valid sudoku, 'guess' a different value.
	//
	//Second check: if the guess finds a valid sudoku puzzle, the second check creates another recursive
	//call of the function using next_row and next_column to move on to the next space.
	// - If true is returned, that means that the sudoku puzzle[9][9] is solved so true is returned.
	// - If false is returned, continue by 'guessing' a different value.
	for (int i = 1; i <= 9; i++)
	{
		puzzle[row_num][column_num] = i;
		if (isValid(puzzle))
		{
			if (solveSudoku(puzzle, next_row, next_column))
				return true;
		}
	}

	//If every value 1-9 has been tried and they have all resulted in invalid puzzles
	//it means that a previous recursive call of solveSudoku is incorrect (or the sudoku is unsolveable).
	//
	//In either of those cases, false needs to be returned to indicate the current puzzle is invalid, and to keep guessing.
	//
	//It is also important to remember to 'clean up' an invalid puzzle by setting all empty spaces to zero
	//so that they don't interfere with previous recursive calls by acting as filled spaces.
	//
	//NOTE: this also functions as a base case if the sudoku puzzle is unsolveable,
	//false is returned back through all recursive calls.
	puzzle[row_num][column_num] = 0;
	return false;
}