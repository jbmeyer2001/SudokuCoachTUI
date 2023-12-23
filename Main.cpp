#include <iostream>
#include <windows.h>

#include "SudokuMap.h"
#include "AlgorithmicSolver.h"
#include "Utility.h"
#include "CheckSudoku.h"

void TUILoop(SudokuMap sudokus);

int main(void)
{
	SudokuMap sudokus;
	TUILoop(sudokus);
	return 0;
}

void TUILoop(SudokuMap sudokus)
{
	std::string name;
	int puzzle[9][9];

get_puzzle:
	std::cout << "puzzle name:";
	std::cin >> name;

	//get the puzzle the user specifies from the sudoku map
	try {
		sudokus.findPuzzle(name)->getPuzzle(puzzle);
	}
	catch (std::runtime_error e) {
		std::cout << "Runtime Error: " << e.what() << std::endl;
		std::cout << "these are the puzzle names available:" << std::endl;
		sudokus.printNames();
		std::cout << std::endl;
		goto get_puzzle;
	}

	//create an instance of AlgorithmicSolver, and check that the puzzle is solveable
	AlgorithmicSolver solver(puzzle);

	StepID id = solver.check();
	switch (id) {
	case StepID::SOLVED:
		break;
	case StepID::CANTSOLVE:
		std::cout << "this solver is incapable of solving this puzzle, although it is a valid puzzle with one solution." << std::endl;
		goto exit;
		break;
	case StepID::UNSOLVEABLE:
		std::cout << "this puzzle is not solveable." << std::endl;
		goto exit;
		break;
	default:
		std::cout << "there's been an internal error." << std::endl;
		goto exit;
	}

	std::cin.ignore();

	while (!solver.solved()) {
		solver.nextStep();
		solver.printStep();
		std::cin.ignore();
	}

	std::cout << "Solved!" << std::endl;
	std::cin.ignore();

exit:
	std::cout << "exiting..." << std::endl;
}