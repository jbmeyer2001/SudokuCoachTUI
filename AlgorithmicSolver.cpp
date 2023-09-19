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
			return true; //TODO: record this to keep track of steps taken
		}
	}

	return false;
}

bool AlgorithmicSolver::uniqueCandidate(void)
{
	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{
		int space = *it;
		int row = space / 9;
		int col = space % 9;
		int box = (space / 27) * 3 + (space % 9) / 3;
		std::set<int> rowSpaces = sameRowColBox(row, -1, -1);
		rowSpaces.erase(space);
		std::set<int> colSpaces = sameRowColBox(-1, col, -1);
		colSpaces.erase(space);
		std::set<int> boxSpaces = sameRowColBox(-1, -1, box);
		boxSpaces.erase(space);

		std::set<int> remainingCandidates;
		remainingCandidates = uniqueCandidateHelper(space, rowSpaces);
		if (remainingCandidates.size() == 1)
		{
			int val = *remainingCandidates.begin();
			puzzle[row][col] = val;
			boardMap->insert(space, val);
			return true; //in the future, record this to keep track of steps taken
		}

		remainingCandidates = uniqueCandidateHelper(space, colSpaces);
		if (remainingCandidates.size() == 1)
		{
			int val = *remainingCandidates.begin();
			puzzle[row][col] = val;
			boardMap->insert(space, val);
			return true; //in the future, record this to keep track of steps taken
		}

		remainingCandidates = uniqueCandidateHelper(space, boxSpaces);
		if (remainingCandidates.size() == 1)
		{
			int val = *remainingCandidates.begin();
			puzzle[row][col] = val;
			boardMap->insert(space, val);
			return true; //in the future, record this to keep track of steps taken
		}
	}

	return false;
}

std::set<int> AlgorithmicSolver::uniqueCandidateHelper(int space, std::set<int> spaces)
{
	std::set<int> candidates = boardMap->spaceCandidates[space];

	std::set<int>::iterator it1;
	for (it1 = spaces.begin(); it1 != spaces.end(); it1++)
	{
		if (boardMap->unfilled.find(*it1) != boardMap->unfilled.end())
		{
			std::set<int> removeCandidates = boardMap->spaceCandidates[*it1];
			std::set<int>::iterator it2;
			for (it2 = removeCandidates.begin(); it2 != removeCandidates.end(); it2++)
			{
				candidates.erase(*it2);
			}
		}	
	}

	return candidates;
}

void AlgorithmicSolver::solve(void)
{
	int i = 0;
	while (!isSolved(puzzle) && i < 80)
	{
		//
		if (soleCandidate()) { continue; }
		if (uniqueCandidate()) { continue; }
		
		i++;
	}

	if (isSolved(puzzle))
	{
		
	}
}