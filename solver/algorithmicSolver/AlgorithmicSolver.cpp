#include <iostream>

#include "AlgorithmicSolver.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9], Step& step)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			this->puzzle[i][j] = puzzle[i][j];
		}
	}
	
	boardMap = new BoardMap(this->puzzle);
	this->step = &step;
}

void AlgorithmicSolver::nextStep(void)
{
	step->clearStep();
	if (soleCandidate()) { return; }
	if (uniqueCandidate()) { return; }
	if (blockColRowInteraction()) { return; }
	if (blockBlockInteraction()) { return; }
	if (nakedSubset()) { return; }
	if (hiddenSubset()) { return; }
	if (XWing()) { return; }

	//update something to indicate we can't find another step
}

//this function is used to test the c++ algorithmic solver implementation
void AlgorithmicSolver::solve(void)
{
	int i = 0;
	
	while (!isSolved(puzzle) && i < 200)
	{
		nextStep();
		std::cout << step->getStep() << std::endl; //for testing purposes
		i++;
	}

	if (isSolved(puzzle))
	{
		printPuzzle(puzzle);
	}
}