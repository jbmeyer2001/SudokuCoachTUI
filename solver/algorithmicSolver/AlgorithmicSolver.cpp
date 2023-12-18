#include <iostream>
#include <Windows.h>

#include "AlgorithmicSolver.h"
#include "CheckSudoku.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9], Step& step)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			this->puzzle[i][j] = puzzle[i][j];
			this->puzzleStart[i][j] = puzzle[i][j];
		}
	}
	
	boardMap.init(puzzle);
	this->step = &step;
}

void AlgorithmicSolver::nextStep(void)
{
	step->clearStep();
	if (solved()) { return; }
	if (soleCandidate()) { return; }
	if (uniqueCandidate()) { return; }
	if (blockColRowInteraction()) { return; }
	if (blockBlockInteraction()) { return; }
	if (nakedSubset()) { return; }
	if (hiddenSubset()) { return; }
	if (XWing()) { return; }
	checkIfSolveable();
}

bool AlgorithmicSolver::solved(void) 
{
	if (isSolved(puzzle))
	{
		step->name = StepID::SOLVED;
		return true;
	}

	return false;
}
StepID AlgorithmicSolver::check(void)
{
	//attempt to solve the puzzle, stop when it's either solved or we realize we can't solve it
	StepID cur = step->getStep();
	while (cur != StepID::SOLVED && cur != StepID::CANTSOLVE && cur != StepID::UNSOLVEABLE) {
		nextStep();
		cur = step->getStep();
	}

	//copy the puzzle's original values back, and reinitialize the board map 
	copyPuzzle(puzzleStart, puzzle);
	boardMap.init(puzzle);

	//set the return value and clear the step
	StepID retval = step->getStep();
	step->clearStep();
	return retval;
}

void AlgorithmicSolver::checkIfSolveable(void)
{
	if (checkSudoku(this->puzzle)) {
		this->step->name = StepID::CANTSOLVE;
	} 
	else {
		this->step->name = StepID::UNSOLVEABLE;
	}
}

void AlgorithmicSolver::printStep(void)
{
	step->printStep(this->puzzle);
}

