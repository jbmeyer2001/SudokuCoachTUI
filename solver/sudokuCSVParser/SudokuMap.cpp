#include <filesystem>

#include "../../include/SudokuMap.h"

/*
function description:
Finds whether a puzzle with a given name is in the linked list.

PARAM: string puzzleName: the name of the puzzle being searched for.
RETURN: sudokuNode pointer, the address of the puzzle being searched for if found, or NULL if no puzzle was found with the provided name.
*/
Sudoku* SudokuMap::findPuzzle(std::string name)
{
	if (!sudokus.contains(name))
		return NULL;

	return &sudokus.find(name)->second;
}

/*
add a sudoku for every file (which are hopefully .csv files!) in the sudokus folder
*/
SudokuMap::SudokuMap(void)
{
	for (const auto& entry : std::filesystem::directory_iterator("sudokus/"))//TODO maybe edit this to be consistent with other iterators, if it can be done
	{
		std::string filename = entry.path().stem().string();
		Sudoku newSudoku(entry.path());
		sudokus.emplace(filename, newSudoku);
	}
}

/*
function description:
Adds a sudoku input by the user to the linked list, creates a text file representation
of the puzzle, and adds the file name of that puzzle to the FilenameList.txt.
*/
void SudokuMap::addSudoku(void)
{
	
	std::string line, filename;
	std::ofstream sudokuListFile, sudokuFile;
	int length, j;
	int puzzle[9][9], puzzleCheckSolved[9][9];
	const char* digit;

	//Recieves input from the user for both the name of the sudoku, and the values in the sudoku.
	//Inludes error handling for if the sudoku file is a duplicate name, and if invalid lines are given.
	std::cout << "          Enter sudoku file name:" << std::endl;
get_sudoku_name:
	getline(std::cin, filename, '\n');
	if (findPuzzle(filename) != NULL)
	{
		std::cout << "          System already has a sudoku with that name, please put in a different file name:" << std::endl;
		goto get_sudoku_name;
	}

	std::cout << "          NOTE: all lines of sudoku puzzle must be entered as 9 integer values seperated by spaces, '0' represents an empty space." << std::endl;
	std::cout << "          Enter sudoku puzzle:" << std::endl;

	for (int i = 0; i < 9; i++)
	{
	read_line_addSudoku:
		getline(std::cin, line, '\n');
		for (int j = 0; j < 9; j++)
		{
			length = line.length();
			if (length != 17)
			{
				std::cout << "          Entry invalid length, try again:" << std::endl;
				goto read_line_addSudoku;
			}

			digit = &line[j * 2];
			try
			{
				puzzle[i][j] = std::stoi(digit);
				puzzleCheckSolved[i][j] = std::stoi(digit);
			}
			catch (std::invalid_argument)
			{
				std::cout << "          Invalid entry, try again:" << std::endl;
				goto read_line_addSudoku;
			}
		}
	}


	//Checks to see whether the puzzle is acutally solveable, and only adds the puzzle to the system if it is.
	if (!solveSudoku(puzzleCheckSolved, 0, 0))
	{
		std::cout << "**ERROR: Sudoku entered does not have a solution, it has not been added to the system**" << std::endl;
	}
	else
	{
		//Creates a sudoku file with the file name given by the user that contains the unsolved puzzle they entered.
		sudokuFile.open(filename + ".csv");
		if (sudokuFile.is_open())
		{
			for (int i = 0; i < 9; i++)
			{
				for (j = 0; j < 8; j++)
				{
					sudokuFile << puzzle[i][j] << ",";
				}
				sudokuFile << puzzle[i][j] << std::endl;
			}
			sudokuFile.close();
		}

		//add sudoku to list
		Sudoku sudoku("sudokus/" + filename + ".csv");
		sudokus.emplace(filename, sudoku);
	}
}

/*
function description:
Displays the names of all puzzles in the linked list.
*/
void SudokuMap::displayPuzzleNames(void)
{
	std::map<std::string, Sudoku>::iterator it;

	for (it = sudokus.begin(); it != sudokus.end(); it++)
	{
		std::cout << it->first << std::endl;
	}
	/*
	SudokuNode* traversal = head;
	std::string name;

	while (traversal != NULL)
	{
		name = traversal->getPuzzleName();
		std::cout << name << std::endl;
		traversal = traversal->next;
	}
	*/
}

/*
function description:
Displays the unsolved representation of a puzzle in the linked list.
*/
void SudokuMap::displayUnsolved(std::string puzzleName)
{
	Sudoku* puzzle = findPuzzle(puzzleName);
	if (puzzle != NULL)
	{
		puzzle->displayUnsolvedPuzzle();
	}
	else
	{
		std::cout << std::endl << "**Sudoku with that name was not found**" << std::endl;
	}
}

/*
function description:
Displays the solved representation of a puzzle in the linked list.
*/
void SudokuMap::displaySolved(std::string puzzleName)
{
	Sudoku* puzzle = findPuzzle(puzzleName);
	if (puzzle != NULL)
	{
		puzzle->displaySolvedPuzzle();
	}
	else
	{
		std::cout << std::endl << "**Sudoku with that name was not found**" << std::endl;
	}
}

/*
	function description:
	Checks a user entered sudoku solution against the actual solution of a sudoku specified
	by the puzzleName parameter, and informs the user whether they solved the puzzle correctly.
*/
void SudokuMap::checkSudoku(std::string puzzleName)
{
	Sudoku* sudoku = findPuzzle(puzzleName);
	if (sudoku == NULL)
	{
		std::cout << std::endl << "**Sudoku with that name was not found**" << std::endl;
		return;
	}

	std::cout << "          NOTE: all lines of sudoku puzzle must be entered as 9 integer values seperated by spaces, '0' represents an empty space." << std::endl;
	std::cout << "          Enter sudoku puzzle:" << std::endl;

	int puzzle[9][9];
	sudoku->getSolvedPuzzle(puzzle);
	std::string line;
	bool wrongFlag = false;

	//Reads the inputs from the user and converts them to integers to be checked against the array,
	//has error handling for if the input is invalid.
	for (int i = 0; i < 9; i++)
	{
	read_line_checkSudoku:
		getline(std::cin, line);
		for (int j = 0; j < 9; j++)
		{
			int length = line.length();
			if (length != 17)
			{
				std::cout << "          Entry invalid length, try again:" << std::endl;
				goto read_line_checkSudoku;
			}
			const char* digit = &line[j * 2];

			try
			{
				if (puzzle[i][j] != std::stoi(digit))
				{
					wrongFlag = true;
				}
			}
			catch (std::invalid_argument)
			{
				std::cout << "          Invalid entry, try again:" << std::endl;
				goto read_line_checkSudoku;
			}
		}
	}

	//Does not inform the user whether the puzzle they entered was incorrect until
	//every line has been entered. If the wrongFlag stays down, it means every 
	//entry was correct, and the user is informed.
	if (!wrongFlag)
	{
		std::cout << "          Congragulations, that was the correct solution!" << std::endl;
	}
	else
		std::cout << "          Unfortunately, that was the incorrect solution." << std::endl;
}

/*
function description:
Deallocate all the memory in the linked list.
*/
SudokuMap::~SudokuMap(void)
{
	sudokus.clear();
}