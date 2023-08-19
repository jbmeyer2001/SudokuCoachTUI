#pragma once

#include "SudokuNode.h"

class SudokuList
{
private:
	SudokuNode* head;

	SudokuNode* findPuzzle(std::string puzzleName);

public:
	SudokuList(void);
	void addSudoku(void);
	void displayPuzzleNames(void);
	void displayUnsolved(std::string puzzleName);
	void displaySolved(std::string puzzleName);
	void checkSudoku(std::string puzzleName);
	~SudokuList(void);
};

