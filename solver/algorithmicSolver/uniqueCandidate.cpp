#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::uniqueCandidate(void)
{
	bool flag = false;

	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{
		int space = *it;
		int row = space / 9;
		int col = space % 9;
		int box = (space / 27) * 3 + (space % 9) / 3;
		std::set<int> rowSpaces = getRow(row);
		rowSpaces.erase(space);
		std::set<int> colSpaces = getCol(col);
		colSpaces.erase(space);
		std::set<int> boxSpaces = getBox(box);
		boxSpaces.erase(space);

		//if one call returns true, the rest won't execute
		flag = flag || checkSpaceUniqueCandidate(space, boxSpaces, row, Set::BOX);
		flag = flag || checkSpaceUniqueCandidate(space, rowSpaces, col, Set::ROW);
		flag = flag || checkSpaceUniqueCandidate(space, colSpaces, box, Set::COL);

		if (flag)
			return true;
	}

	return false;
}

bool AlgorithmicSolver::checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set set)
{
	std::set<int> candidates = boardMap->spaceCandidates[space];

	std::set<int>::iterator it;
	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		int val = *it;
		if (boardMap->unfilled.contains(val))
		{
			std::set<int> removeCandidates = boardMap->spaceCandidates[val];
			candidates = getDifference(candidates, removeCandidates);
		}
	}

	if (candidates.size() == 1)
	{
		int val = *candidates.begin();

		puzzle[space / 9][space % 9] = val;
		boardMap->insert(space, val);

		//record step/technique used to get this square
		step->updateUniqueCandidate(space / 9, space % 9, val, subset, set);

		//return true, meaning we've made changes to the puzzle and updated the step
		return true;
	}

	return false;
}