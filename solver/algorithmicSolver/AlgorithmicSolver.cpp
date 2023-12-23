#include <iostream>
#include <Windows.h>

#include "AlgorithmicSolver.h"
#include "CheckSudoku.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			this->puzzle[i][j] = puzzle[i][j];
			this->puzzleStart[i][j] = puzzle[i][j];
		}
	}
	
	boardMap.init(puzzle);
}

void AlgorithmicSolver::nextStep(void)
{
	//go through all possible steps starting from (relatively) the easier 
	//ones to see, and search for harder patterns as we go on.
	step.clearStep();
	if (solved()) { return; }
	if (soleCandidate()) { return; }
	if (uniqueCandidate()) { return; }
	if (blockRowColInteraction()) { return; }
	if (blockBlockInteraction()) { return; }
	if (nakedSubset()) { return; }
	if (hiddenSubset()) { return; }
	if (XWing()) { return; }
	if (YWing()) { return; }
	checkIfSolveable();
}

bool AlgorithmicSolver::solved(void) 
{
	if (isSolved(puzzle)) {
		step.solved();
		return true;
	}

	return false;
}

StepID AlgorithmicSolver::check(void)
{
	//attempt to solve the puzzle, stop when it's either solved or we realize we can't solve it
	StepID cur = step.getStepID();
	while (cur != StepID::SOLVED && cur != StepID::CANTSOLVE && cur != StepID::UNSOLVEABLE) {
		nextStep();
		cur = step.getStepID();
	}

	//copy the puzzle's original values back, and reinitialize the board map 
	copyPuzzle(puzzleStart, puzzle);
	boardMap.init(puzzle);

	//set the return value and clear the step
	StepID retval = step.getStepID();
	step.clearStep();
	return retval;
}

void AlgorithmicSolver::checkIfSolveable(void)
{
	//checkSudoku returns true if there is a single valid solution to the puzzle given
	if (checkSudoku(this->puzzle)) {
		this->step.cantSolve();
	} 
	else {
		this->step.unsolveable();
	}
}

void AlgorithmicSolver::printStep(void)
{
	step.printStep(this->puzzle);
}

