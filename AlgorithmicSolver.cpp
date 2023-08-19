#include "AlgorithmicSolver.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			unsolvedPuzzle[i][j] = puzzle[i][j];
		}
	}
}