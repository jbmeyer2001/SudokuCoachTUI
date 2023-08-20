#include "AlgorithmicSolver.h"
#include "BoardMap.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int val = puzzle[i][j];

			this->puzzle[i][j] = val;
			
			if (val == -1)
				unfilled.insert(i * 9 + j);
		}
	}
	
	boardMap = BoardMap(this->puzzle);
}

void AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	int mustBe = 0;
	for (it = unfilled.begin(); it != unfilled.end(); it++)
	{
		mustBe = boardMap.mustBe();
		if (mustBe != -1)
		{

		}
	}
}