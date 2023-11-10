#include <unordered_map>
#include <string>
#include <iostream>

#include "../../include/AlgorithmicSolver.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int val = puzzle[i][j];
			this->puzzle[i][j] = val;
		}
	}
	
	boardMap = new BoardMap(this->puzzle);
}





void AlgorithmicSolver::nextStep(void)
{
	clearStepInfo();
	if (soleCandidate()) { return; }
	if (uniqueCandidate()) { return; }
	if (blockColRowInteraction()) { return; }
	if (blockBlockInteraction()) { return; }
	if (nakedSubset()) { return; }
	if (hiddenSubset()) { return; }
	if (XWing()) { return; }

	//update something to indicate we can't find another step
}

void AlgorithmicSolver::solve(void)
{
	int i = 0;
	
	//super jank, change later
	while (!isSolved(puzzle) && i < 200)
	{
		nextStep();
		std::cout << getStep() << std::endl; //for testing purposes
		i++;
	}

	if (isSolved(puzzle))
	{
		printPuzzle(puzzle);
	}
}