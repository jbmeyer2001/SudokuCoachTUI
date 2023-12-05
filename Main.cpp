#include "SudokuMap.h"
#include "AlgorithmicSolver.h"
#include "Utility.h"
#include "CheckSudoku.h"

int main(void)
{
	SudokuMap sudokus;
	Sudoku* sudoku = sudokus.findPuzzle("XWingTest");
	
	if (sudoku == NULL)
		return 0;

	int puzzle[9][9];
	sudoku->getPuzzle(puzzle); 

	Step step;
	AlgorithmicSolver solver(puzzle, step);
	solver.solve();
}