#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include "Utility.h"
#include "RecursiveSolver.h"

class SudokuNode
{
private:
	std::string puzzleName = "";
	bool puzzleAssignedFlag = false;
	int puzzleUnsolved[9][9] = { -1 };
	int puzzleSolved[9][9] = { -1 };

public:
	SudokuNode* next = NULL;
	void assignPuzzle(std::string puzzleName);
	void displayUnsolvedPuzzle(void);
	void displaySolvedPuzzle(void);
	void getSolvedPuzzle(int puzzle[9][9]);
	void getUnsolvedPuzzle(int puzzle[9][9]);
	std::string getPuzzleName(void);
};

