#include "SudokuMap.h"
#include "AlgorithmicSolver.h"
#include "Utility.h"

int main(void)
{
	//TODO: check for puzzle validity somewhere, if it's not already being done
	SudokuMap sudokus;
	Sudoku* sudoku = sudokus.findPuzzle("XWingTest");
	
	if (sudoku == NULL)
		return 0;

	int puzzle[9][9];
	sudoku->getUnsolvedPuzzle(puzzle);

	AlgorithmicSolver solver(puzzle);
	solver.solve();
}