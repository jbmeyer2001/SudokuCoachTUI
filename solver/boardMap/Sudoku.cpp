#include "Sudoku.h"

/*
takes a path to a csv file and generates a sudoku puzzle (9x9 array of ints).
this function is only used by me to test other parts of the project, and isn't
meant to be deployed in any way, as such I haven't included error checking.
*/
Sudoku::Sudoku(std::filesystem::path path)
{
	int number = -1;
	bool errFlag = false;

	std::string path_string = path.string();
	std::ifstream file;
	file.open(path_string);

	if (!file.is_open()) {
		std::cout << "file not open" << std::endl;
		return;
	}

	std::string line;
	int i = 0;

	for (int i = 0; i < 9; i++) {
		std::getline(file, line);

		for (int j = 0; j < 9; j++) {
   			int val = line.at(j * 2) - '0';
			puzzle[i][j] = val;
		}
	}

	file.close();
}

void Sudoku::getPuzzle(int puzzle[9][9])
{
	copyPuzzle(this->puzzle, puzzle);
}

