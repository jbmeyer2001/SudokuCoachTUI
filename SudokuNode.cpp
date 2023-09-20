#include "SudokuNode.h"

/*
function description:
Fills the properties of sudokuNode object with values that are determined from
a text file. The text file is found by its name through the parameter 'puzzleName'
and is formatted as nine lines of nine values (from 0-9) seperated by spaces,
where '0' represents an empty space.

PARAM: String puzzleName: represents the file name of a text file that contains
a sudoku puzzle.
*/
void SudokuNode::assignPuzzle(std::string name)
{
	int number = -1;
	bool errFlag = false;

	//Only allow object to be assigned if it has not been assigned previously.
	if (!puzzleAssignedFlag)
	{
		std::string directory = "sudokus/" + name + ".csv";
		std::ifstream file;
		file.open(directory);

		std::string line;
		int i = 0;

		for (int i = 0; i < 9; i++) //TODO what if file doesn't have this many lines!!!
		{
			std::getline(file, line);

			for (int j = 0; j < 9; j++)
			{
				int val = line.at(j * 2) - '0';
				puzzleUnsolved[i][j] = val;
				puzzleSolved[i][j] = val;
			}

			i++;
		}

		file.close();

		solveSudoku(puzzleSolved, 0, 0); //TODO add something for if there is an error in the puzzle
		puzzleAssignedFlag = true;

		//TODO error handling on this entire class, so much error handling
	}
}

/*
function description:
If the puzzle has been assigned, calls printPuzzle on the puzzle_unsolved array.
*/
void SudokuNode::displayUnsolvedPuzzle(void)
{
	std::cout << std::endl;
	if (puzzleAssignedFlag)
	{
		printPuzzle(puzzleUnsolved);
	}
	else
		std::cout << "**Error: Program attempted to access puzzle that has not been assigned**" << std::endl;
}

/*
function description:
If the puzzle has been assigned, calls printPuzzle on the puzzle_solved array.
*/
void SudokuNode::displaySolvedPuzzle(void)
{
	std::cout << std::endl;
	if (puzzleAssignedFlag)
	{
		printPuzzle(puzzleSolved);
	}
	else
		std::cout << "**Error: Program attempted to access puzzle that has not been assigned**" << std::endl;
}

/*
function description:
If the puzzle has been assigned, copies the values from puzzle_solved into the puzzleparameter.

PARAM: 9x9 integer array puzzle[9][9]: solved puzzle copied to this parameter.
*/
void SudokuNode::getSolvedPuzzle(int puzzle[9][9])
{
	if (puzzleAssignedFlag)
	{
		copyPuzzle(puzzleSolved, puzzle);
	}
	else
		std::cout << "**Error: Program attempted to access puzzle that has not been assigned**" << std::endl;
}

void SudokuNode::getUnsolvedPuzzle(int puzzle[9][9])
{
	if (puzzleAssignedFlag)
	{
		copyPuzzle(puzzleUnsolved, puzzle);
	}
	else
		std::cout << "**Error: Program attempted to access puzzle that has not been assigned**" << std::endl;
}

/*
function description
If the puzzle has been assigned, returns the puzzlename.

RETURN: string representing name of puzzle.
*/
std::string SudokuNode::getPuzzleName(void)
{
	if (puzzleAssignedFlag)
	{
		return puzzleName;
	}
	else
	{
		std::cout << "**Error: Program attempted to access puzzle that has not been assigned**" << std::endl;
		return "";
	}

}
