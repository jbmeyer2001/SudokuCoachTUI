#include <algorithm>

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
			return true; //TODO: record this to keep track of steps taken
		}

		remainingCandidates = uniqueCandidateHelper(space, colSpaces);
		if (remainingCandidates.size() == 1)
		{
			int val = *remainingCandidates.begin();
			puzzle[row][col] = val;
			boardMap->insert(space, val);
			return true; //TODO: record this to keep track of steps taken
		}

		remainingCandidates = uniqueCandidateHelper(space, boxSpaces);
		if (remainingCandidates.size() == 1)
		{
			int val = *remainingCandidates.begin();
			puzzle[row][col] = val;
			boardMap->insert(space, val);
			return true; //TODO: record this to keep track of steps taken
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

bool AlgorithmicSolver::blockColRowInteraction(void)
{
	for (int box = 0; box < 9; box++) //for each block
	{
		int startIndex = (box / 3) * 27 + (box % 3) * 3;

		/*
		get commonalities of each row/column within the box

		a commonality is defines as a number that appears at least
		twice in any given row or column within the box
		*/
		
		std::set<int>::iterator it;

		//start with rows
		std::set<int> rowOneCommonalities = getCommonalities(startIndex, startIndex + 1, startIndex + 2);
		std::set<int> rowTwoCommonalities = getCommonalities(startIndex + 9, startIndex + 10, startIndex + 11);
		std::set<int> rowThreeCommonalities = getCommonalities(startIndex + 18, startIndex + 19, startIndex + 20);

		for (it = rowOneCommonalities.begin(); it != rowOneCommonalities.end(); it++)
		{
			if (!rowTwoCommonalities.contains(*it) && !rowThreeCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}

		for (it = rowTwoCommonalities.begin(); it != rowTwoCommonalities.end(); it++)
		{
			if (!rowOneCommonalities.contains(*it) && !rowThreeCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}

		for (it = rowThreeCommonalities.begin(); it != rowThreeCommonalities.end(); it++)
		{
			if (!rowOneCommonalities.contains(*it) && !rowTwoCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}

		//next do the columns
		std::set<int> colOneCommonalities = getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		for (it = colOneCommonalities.begin(); it != colOneCommonalities.end(); it++)
		{
			if (!colTwoCommonalities.contains(*it) && !colThreeCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}

		for (it = colTwoCommonalities.begin(); it != colTwoCommonalities.end(); it++)
		{
			if (!colOneCommonalities.contains(*it) && !colThreeCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}

		for (it = colThreeCommonalities.begin(); it != colThreeCommonalities.end(); it++)
		{
			if (!colOneCommonalities.contains(*it) && !colTwoCommonalities.contains(*it))
			{
				//remove the candidate from all other indexes in the row (not counting the ones in this box)
				//record that this happened
			}
		}
	}
}

std::set<int> AlgorithmicSolver::getCommonalities(int i1, int i2, int i3)
{
	std::set<int> commonalities;
	std::set<int> si1 = boardMap->spaceCandidates[i1]; //TODO maybe come up with better variable names than si1, si2, si3
	std::set<int> si2 = boardMap->spaceCandidates[i2];
	std::set<int> si3 = boardMap->spaceCandidates[i3];
	
	//i1 & i2
	std::set_intersection(si1.begin(), si1.end(), si2.begin(), si2.end(), commonalities.begin());

	//i1 & i3
	std::set_intersection(si1.begin(), si1.end(), si3.begin(), si3.end(), commonalities.end()); //TODO: not sure if  this .end() business is going to work

	//i2 & i3
	std::set_intersection(si2.begin(), si2.end(), si3.begin(), si3.end(), commonalities.end());

	return commonalities;
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