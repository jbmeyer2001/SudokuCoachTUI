#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::hiddenSubset(void)
{
	Set set;
	std::set<int> spaces;

	for (int i = 0; i < 27; i++)
	{
		switch (i % 9)
		{
		case 0:
			spaces = boardMap->getUnfilledSpaces(getRow(i / 9));
			set = Set::ROW;
			break;
		case 1:
			spaces = boardMap->getUnfilledSpaces(getCol(i / 9));
			set = Set::COL;
			break;
		case 2:
			spaces = boardMap->getUnfilledSpaces(getBox(i / 9));
			set = Set::BOX;
			break;
		}

		if (spaces.size() < 4)
			continue;

		unsigned short prev = (unsigned short)2;
		std::vector<int> spacesVec(spaces.begin(), spaces.end());

		std::set<int> partition = getNextPartition(spacesVec, prev);
		while (!partition.empty())
		{
			std::set<int> other = getDifference(spaces, partition);

			std::set<int> partitionCandidates = boardMap->getCandidates(partition);
			std::set<int> otherCandidates = boardMap->getCandidates(other);

			if (getDifference(partitionCandidates, otherCandidates).size() == partition.size())
			{
				int unused = 0;
				if (boardMap->removeCandidates(otherCandidates, partition))
				{
					step->updateHiddenSubset(i / 9, set, partition, otherCandidates);
					return true;
				}
			}

			partition = getNextPartition(spacesVec, prev);
		}
	}

	return false;
}

std::set<int> AlgorithmicSolver::getNextPartition(std::vector<int> spaces, unsigned short& prev)
{
	std::set<int> retval;
	uint16_t curInt;
	unsigned short cur;

getPartition:
	curInt = (uint16_t)prev + 1;
	cur = (unsigned short)curInt;

	if (curInt >= pow(2, spaces.size()) - 1)
	{
		return retval;
	}

	for (uint16_t i = 0; i < spaces.size(); i++)
	{
		uint16_t mask = pow(2, i);

		if ((prev & mask) == mask)
		{
			retval.insert(spaces[i]);
		}
	}

	if (retval.size() < 2 || retval.size() > (spaces.size() - 2))
	{
		retval.clear();
		prev = cur;
		goto getPartition;
	}

	prev = cur;
	return retval;
}