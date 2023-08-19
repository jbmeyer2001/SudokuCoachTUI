#include <iostream>
#include "Utility.h"

/*
	function description:
	Checks a 9x9 array that represents a sudoku puzzle to see whether it is solved or not.

	PARAM: 9x9 integer array puzzle[9][9]: array representing a sudoku puzzle.
	RETURN: boolean variable: true if the sudoku is solved and zero if it is not.
*/
bool isSolved(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puzzle[i][j] == 0)
				return false;
		}
	}
	return true;
}

/*
	function description:
	Checks an array with 9 values to see whether there are duplicates.

	PARAM: integer array array[9]: array representing a line, column, or box in a sudoku puzzle.
	RETURN: boolean value: true if there are duplicates, false if there are no duplicates.
*/
bool duplicates(int array[9])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 8; j > i; j--)
		{
			//----------------------------------------------------------------------//
			//Index i starts at the beginning, and for each iteration of index i,
			//index j starts at the end and moves towards the beginning until it 
			//reaches i. This ensures that every index is checked against every other,
			//but never against itself.
			//
			//Indices filled with 0 (empty spaces) are ignored.
			//----------------------------------------------------------------------//
			if ((array[i] != 0) && (array[j] != 0) && (array[i] == array[j]))
				return true;
		}
	}
	return false;
}

/*
	function description:
	Checks a 9x9 array that represents a sudoku puzzle to ensure it is valid.
	It first checks to see if there are any invalid numbers, or any numbers less than zero or more than nine.
	Then, it checks each row, column, and box to see if there are any duplicates.

	PARAM: 9x9 integer array puzzle[9][9]: array representing a sudoku puzzle.
	RETURN: boolean variable: true if the sudoku is valid, and zero if it is not.
*/
bool isValid(int puzzle[9][9])
{
	int checkArray[9];

	//Check to see if there are any negatives or numbers over 9.
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((puzzle[i][j] > 9) || (puzzle[i][j] < 0))
			{
				return false;
			}
		}
	}

	//Check all rows to see if they have duplicates.
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			checkArray[j] = puzzle[i][j];
		}
		if (duplicates(checkArray))
		{
			return false;
		}
	}

	//Check all columns to see if they have duplicates.
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			checkArray[j] = puzzle[j][i];
		}
		if (duplicates(checkArray))
		{
			return false;
		}
	}

	//Check all boxes to see if they have duplicates.
	for (int i = 0; i <= 6; i = i + 3)
	{
		for (int j = 0; j <= 6; j = j + 3)
		{
			//---------------------------------------------------------------------------------//
			//Variables i and j determine which box is being checked for duplicates by selecting
			//the first index of each box within the sudoku array. (0,0) for box one, (0,3)
			//for box two, (0,6) for box 3, (3,0) for box four, etc.
			//---------------------------------------------------------------------------------//
			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++) {
					//-------------------------------------------------------------------------------------------------------------------------//
					//Variables k and l determine the indices within each box, and therefore, the values copied into checkArray[9] to be checked.
					//
					//For example, the indices of box 5 are: (3,3), (3,4), (3,5), (4,3), (4,4), (4,5), (5,3), (5,4), (5,5).
					//-------------------------------------------------------------------------------------------------------------------------//
					checkArray[(k * 3) + l] = puzzle[k + i][l + j];
				}
			}
			if (duplicates(checkArray))
			{
				return false;
			}
		}
	}

	return true;
}

/*
	function description:
	Prints the sudoku puzzle with spaces between every value, and an endline after every row.

	PARAM: 9x9 integer array puzzle[9][9]: array representing a sudoku puzzle.
*/
void printPuzzle(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

/*
	function description:
	Used by the recursive sudoku algorithm to decide what the row index is for the next recursive call.

	PARAM: integer row_num: index of the current row.
	PARAM: integer column_num, index of the current column.
	RETURN: integer, the row index for the next recursive call.
*/
int nextRow(int row_num, int column_num)
{
	//Row should increase only if the current function call is on the last column.
	if (column_num == 8)
	{
		row_num++;
		return row_num;
	}
	return row_num;
}

/*
	function description:
	Used by the recursive sudoku algorithm to decide what the column index is for the next recursive call.

	PARAM: integer row_num: index of the current row.
	PARAM: integer column_num: index of the current column.
	RETURN: integer, the column index for the next recursive call.
*/
int nextColumn(int row_num, int column_num)
{
	//If the current function call is on the last column, reset to the first column (of the next row).
	//Increment the column in every other case to continue traversing a row.
	if (column_num == 8)
	{
		return  0;
	}
	else
	{
		column_num++;
		return column_num;
	}
}

void copyPuzzle(int fullPuzzle[9][9], int emptyPuzzle[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			emptyPuzzle[i][j] = fullPuzzle[i][j];
		}
	}
}