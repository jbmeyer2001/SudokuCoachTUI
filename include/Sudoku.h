#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Utility.h"

class Sudoku
{
private:
	std::string name = "";
	int puzzle[9][9] = { -1 };

public:
	Sudoku(std::filesystem::path path);
	void getPuzzle(int puzzle[9][9]);
};

