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
			
			if (val == -1 || val == 0)
				unfilled.insert(i * 9 + j);
		}
	}
	
	boardMap = new BoardMap(this->puzzle);
}

bool AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	int mustBe = 0;
	for (it = unfilled.begin(); it != unfilled.end(); it++)
	{
		int spaceNum = *it;
		mustBe = boardMap->mustBe(spaceNum);
		if (mustBe != -1)
		{
			int row = spaceNum / 9;
			int col = spaceNum % 9;

			puzzle[row][col] = mustBe;
			boardMap->insert(spaceNum, mustBe);
			unfilled.erase(spaceNum);
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