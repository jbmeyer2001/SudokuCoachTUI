#pragma once

#include <map>
#include <string>

#include "Sudoku.h"

class SudokuMap
{
private:
	std::map<std::string, Sudoku> sudokus;
	Sudoku* findPuzzle(std::string puzzleName);

public:
	SudokuMap(void);
	void addSudoku(void);
	void displayPuzzleNames(void);
	void displayUnsolved(std::string puzzleName);
	void displaySolved(std::string puzzleName);
	void checkSudoku(std::string puzzleName);
	~SudokuMap(void);
};

