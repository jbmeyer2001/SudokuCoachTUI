#include "Sudoku.h"

/*
function description:
Fills the properties of sudokuNode object with values that are determined from
a text file. The text file is found by its name through the parameter 'puzzleName'
and is formatted as nine lines of nine values (from 0-9) seperated by spaces,
where '0' represents an empty space.

PARAM: String puzzleName: represents the file name of a text file that contains
a sudoku puzzle.
*/
Sudoku::Sudoku(std::filesystem::path path)
{
	int number = -1;
	bool errFlag = false;

	std::string s_path = path.string();
	std::ifstream file;
	file.open(s_path); //TODO check if file is actually open

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

	//TODO error handling on this entire class, so much error handling
	
}

//TODO update all of the function descriptions to remove "if puzzle assigned"

/*
function description:
If the puzzle has been assigned, calls printPuzzle on the puzzle_unsolved array.
*/
void Sudoku::displayUnsolvedPuzzle(void)
{
	std::cout << std::endl;
	printPuzzle(puzzleUnsolved);
}

/*
function description:
If the puzzle has been assigned, calls printPuzzle on the puzzle_solved array.
*/
void Sudoku::displaySolvedPuzzle(void)
{
	std::cout << std::endl;
	printPuzzle(puzzleSolved);
}

/*
function description:
If the puzzle has been assigned, copies the values from puzzle_solved into the puzzleparameter.

PARAM: 9x9 integer array puzzle[9][9]: solved puzzle copied to this parameter.
*/
void Sudoku::getSolvedPuzzle(int puzzle[9][9])
{
	copyPuzzle(puzzleSolved, puzzle);
}

void Sudoku::getUnsolvedPuzzle(int puzzle[9][9])
{
	copyPuzzle(puzzleUnsolved, puzzle);
}

