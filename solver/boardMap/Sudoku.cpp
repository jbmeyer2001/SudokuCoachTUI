#include "Sudoku.h"

/*
takes a path to a csv file and generates a sudoku puzzle (9x9 array of ints).
Check each csv file while generating the sudoku to ensure it is formatted correctly.
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

		if (line.length() != 17) {
			throw std::runtime_error(".csv line length incorrect");
		}
		for (int j = 0; j < 17; j++) {

			if (j % 2 == 1) {
				if (line.at(j) != ',') {
					throw std::runtime_error("a comma needs to be between every number");
				}
				continue;
			}

			int val = line.at(j) - '0';
			if (j % 2 == 0 && (val < 0 || val > 9)) {
				throw std::runtime_error("the entries need to be integers between 0 and 9");
			}
   			
			puzzle[i][j / 2] = val;
		}
	}

	file.close();
}

void Sudoku::getPuzzle(int puzzle[9][9])
{
	copyPuzzle(this->puzzle, puzzle);
}

