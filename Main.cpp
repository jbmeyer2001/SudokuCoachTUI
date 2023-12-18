#include <iostream>
#include <windows.h>

#include "SudokuMap.h"
#include "AlgorithmicSolver.h"
#include "Utility.h"
#include "CheckSudoku.h"

void TUILoop(SudokuMap sudokus);

int main(void)
{
	/*
	SudokuMap sudokus;
	Sudoku* sudoku = sudokus.findPuzzle("XWingTest");
	
	if (sudoku == NULL)
		return 0;

	int puzzle[9][9];
	sudoku->getPuzzle(puzzle); 

	Step step;
	AlgorithmicSolver solver(puzzle, step);
	solver.solve();
	*/

	//throw and catch an error here
	SudokuMap sudokus;
	TUILoop(sudokus);
	return 0;
}

void TUILoop(SudokuMap sudokus)
{
	std::string name;

	std::cout << "puzzle name:";
	std::cin >> name;

	//add throw and catch here
	Sudoku* sudoku = sudokus.findPuzzle(name);

	Step step;
	int puzzle[9][9];
	sudoku->getPuzzle(puzzle);
	AlgorithmicSolver solver(puzzle, step);

	StepID id = solver.check();
	switch (id)
	{
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

	while (step.getStep() != StepID::SOLVED) {
		solver.nextStep();
		solver.printStep();
		std::cin.ignore();
	}

exit:
	std::cout << "exiting..." << std::endl;
	//free stuff
}