#include "AlgorithmicSolver.h"
#include "BoardMap.h"
#include "Utility.h"

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

bool AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{

		int spaceNum = *it;
		std::set<int> candidates = boardMap->spaceCandidates[spaceNum];
		if (candidates.size() == 1)
		{
			int row = spaceNum / 9;
			int col = spaceNum % 9;
			int val = *candidates.begin();

			puzzle[row][col] = val;
			boardMap->insert(spaceNum, val);
			return true;
		}
	}

	return false;
}

void AlgorithmicSolver::solve(void)
{
	int i = 0;
	while (!isSolved(puzzle) && i < 80)
	{
		//
		soleCandidate();
		i++;
	}

	if (isSolved(puzzle))
	{
		
	}
}