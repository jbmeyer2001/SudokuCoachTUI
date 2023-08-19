#include "SudokuList.h"

/*
function description:
Finds whether a puzzle with a given name is in the linked list.

PARAM: string puzzleName: the name of the puzzle being searched for.
RETURN: sudokuNode pointer, the address of the puzzle being searched for if found, or NULL if no puzzle was found with the provided name.
*/
SudokuNode* SudokuList::findPuzzle(std::string name)
{
	SudokuNode* traversal = head;

	//Checks each node in the linked list to see if the puzzleName parameter matches the puzzleName of the node
	//and returns a pointer to that node if it does. Returns the NULL pointer if a node with puzzleName was not found.
	if (name == traversal->getPuzzleName())
	{
		return traversal;
	}
	else
	{
		do
		{
			traversal = traversal->next;
			if (name == traversal->getPuzzleName())
			{
				return traversal;
			}
		} while (traversal->next != NULL);
	}
	return NULL;
}

/*
function description:
Creates the head node for the list, and fills in the linked list with all the
puzzles specified in the FilenameList.txt file.
*/
SudokuList::SudokuList(void)
{
	std::string puzzleName;
	std::ifstream sudokuListFile;

	//Defines the head node pointer, allocates the head node,
	//and assigns it the puzzle specified by the first line in FilenameList.txt.
	SudokuNode* newNode;
	SudokuNode* traversalNode;
	head = new SudokuNode;

	sudokuListFile.open("sudokus/FilenameList.txt");
	getline(sudokuListFile, puzzleName);

	head->assignPuzzle(puzzleName);
	traversalNode = head;

	//Iterates through all lines in the FilenameList.txt file, 
	//allocating and assigning nodes of sudokuNode.
	while (getline(sudokuListFile, puzzleName))
	{
		newNode = new SudokuNode;
		newNode->assignPuzzle(puzzleName);
		traversalNode->next = newNode;
		traversalNode = newNode;
	}
}

/*
function description:
Adds a sudoku input by the user to the linked list, creates a text file representation
of the puzzle, and adds the file name of that puzzle to the FilenameList.txt.
*/
void SudokuList::addSudoku(void)
{
	std::string line, filename;
	std::ofstream sudokuListFile, sudokuFile;
	SudokuNode* traversal;
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

	std::cout << "          NOTE: all lines of sudoku puzzle must be entered as 9 integer values seperated by spaces, '0' represents an empty square." << std::endl;
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
		sudokuFile.open(filename + ".txt");
		if (sudokuFile.is_open())
		{
			for (int i = 0; i < 9; i++)
			{
				for (j = 0; j < 8; j++)
				{
					sudokuFile << puzzle[i][j] << " ";
				}
				sudokuFile << puzzle[i][j] << std::endl;
			}
			sudokuFile.close();
		}

		//Appends the file name into the FilenameList.txt file.
		sudokuListFile.open("FilenameList.txt", std::ios::app);
		if (sudokuListFile.is_open())
		{
			sudokuListFile << "\n" << filename;
			sudokuListFile.close();
		}

		//Allocates and assigns a new sudokuNode with the information given by the user,
		//and traverses through the linked list to add it to the end.
		traversal = head;
		while (traversal->next != NULL)
		{
			traversal = traversal->next;
		}
		traversal->next = new SudokuNode;
		traversal = traversal->next;
		traversal->assignPuzzle(filename);

		std::cout << "           " << filename << " has been successfully added!" << std::endl;
	}
}

/*
function description:
Displays the names of all puzzles in the linked list.
*/
void SudokuList::displayPuzzleNames(void)
{
	SudokuNode* traversal = head;
	std::string name;

	while (traversal != NULL)
	{
		name = traversal->getPuzzleName();
		std::cout << name << std::endl;
		traversal = traversal->next;
	}
}

/*
function description:
Displays the unsolved representation of a puzzle in the linked list.
*/
void SudokuList::displayUnsolved(std::string puzzleName)
{
	SudokuNode* sudoku;
	sudoku = findPuzzle(puzzleName);
	if (sudoku != NULL)
	{
		sudoku->displayUnsolvedPuzzle();
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
void SudokuList::displaySolved(std::string puzzleName)
{
	SudokuNode* sudoku;
	sudoku = findPuzzle(puzzleName);
	if (sudoku != NULL)
	{
		sudoku->displaySolvedPuzzle();
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
void SudokuList::checkSudoku(std::string puzzleName)
{
	SudokuNode* sudoku;
	int puzzle[9][9];
	int length;
	const char* digit;
	std::string line;
	bool wrongFlag = false;


	sudoku = findPuzzle(puzzleName);
	sudoku->getSolvedPuzzle(puzzle);

	std::cout << "          NOTE: all lines of sudoku puzzle must be entered as 9 integer values seperated by spaces, '0' represents an empty square." << std::endl;
	std::cout << "          Enter sudoku puzzle:" << std::endl;

	//Reads the inputs from the user and converts them to integers to be checked against the array,
	//has error handling for if the input is invalid.
	for (int i = 0; i < 9; i++)
	{
	read_line_checkSudoku:
		getline(std::cin, line);
		for (int j = 0; j < 9; j++)
		{
			length = line.length();
			if (length != 17)
			{
				std::cout << "          Entry invalid length, try again:" << std::endl;
				goto read_line_checkSudoku;
			}
			digit = &line[j * 2];

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
SudokuList::~SudokuList(void)
{
	SudokuNode* traversal;
	SudokuNode* next;

	traversal = head;
	next = traversal->next;

	while (traversal->next != NULL)
	{
		delete traversal;
		traversal = next;
		next = traversal->next;
	}
	delete traversal;
}