#pragma once

#include <map>
#include <string>

#include "Sudoku.h"

class SudokuMap
{
private:
	std::map<std::string, Sudoku> sudokus;
	
public:
	SudokuMap(void);
	Sudoku* findPuzzle(std::string puzzleName);
	~SudokuMap(void);
};

