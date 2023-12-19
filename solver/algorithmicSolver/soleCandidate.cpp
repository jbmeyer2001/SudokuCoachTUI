#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	for (it = boardMap.unfilled.begin(); it != boardMap.unfilled.end(); it++) {
		int space = *it;
		std::set<int> candidates = boardMap.getCandidates(space);
		if (candidates.size() == 1) {
			int row = space / 9;
			int col = space % 9;
			int val = *candidates.begin();

			//place number into puzzle
			puzzle[space / 9][space % 9] = val;
			boardMap.insert(space, val);

			//record step/technique used to get this square
			step.updateSoleCandidate(row, col, val);

			//return true, meaning we've made changes to the puzzle and updated the step
			return true;
		}
	}

	return false;
}