#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Utility.h"
#include "RecursiveSolver.h"

class Sudoku
{
private:
	std::string puzzleName = "";
	int puzzleUnsolved[9][9] = { -1 };
	int puzzleSolved[9][9] = { -1 };

public:
	Sudoku(std::filesystem::path path);
	void displayUnsolvedPuzzle(void);
	void displaySolvedPuzzle(void);
	void getSolvedPuzzle(int puzzle[9][9]);
	void getUnsolvedPuzzle(int puzzle[9][9]);
};

