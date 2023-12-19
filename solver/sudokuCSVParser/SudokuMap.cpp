#include <filesystem>

#include "SudokuMap.h"

/*
add a sudoku for every file in the sudokus folder
*/
SudokuMap::SudokuMap(void)
{
	for (const auto& entry : std::filesystem::directory_iterator("sudokus/")) {
		std::string filename = entry.path().string();

		if (filename.substr(filename.find_last_of(".") + 1) == "csv") {
			try {
				//remove 'sudokus/' from front of string
				std::string name = filename.substr(filename.find_first_of("/") + 1);

				//remove '.csv' from end of string
				name = name.substr(0, name.length() - 4);

				Sudoku newSudoku(entry.path());
				sudokus.emplace(name, newSudoku);
			}
			catch (std::runtime_error e) {
				// don't add the badly formatted sudoku to the map
			}
		}
	}
}

Sudoku* SudokuMap::findPuzzle(std::string name)
{
	if (!sudokus.contains(name)) {
		throw std::runtime_error("a sudoku with that name does not exist");
	}

	return &sudokus.find(name)->second;
}

void SudokuMap::printNames(void) {
	std::map<std::string, Sudoku>::iterator it;

	for (it = sudokus.begin(); it != sudokus.end(); it++) {
		std::cout << it->first << std::endl;
	}
}

SudokuMap::~SudokuMap(void)
{
	sudokus.clear();
}