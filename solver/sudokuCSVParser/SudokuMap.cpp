#include <filesystem>

#include "SudokuMap.h"

/*
add a sudoku for every file (which are hopefully .csv files!) in the sudokus folder
*/
SudokuMap::SudokuMap(void)
{
	for (const auto& entry : std::filesystem::directory_iterator("sudokus/"))
	{
		std::string filename = entry.path().stem().string();
		Sudoku newSudoku(entry.path());
		sudokus.emplace(filename, newSudoku);
	}
}

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
function description:
Deallocate all the memory in the linked list.
*/
SudokuMap::~SudokuMap(void)
{
	sudokus.clear();
}