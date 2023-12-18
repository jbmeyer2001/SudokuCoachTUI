#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::nakedSubset(void)
{
	Set set;
	std::set<int> spaces;

	for (int i = 0; i < 27; i++)
	{
		switch (i % 9)
		{
		case 0:
			spaces = boardMap.getUnfilledSpaces(getRow(i / 9));
			set = Set::ROW;
			break;
		case 1:
			spaces = boardMap.getUnfilledSpaces(getCol(i / 9));
			set = Set::COL;
			break;
		case 2:
			spaces = boardMap.getUnfilledSpaces(getBox(i / 9));
			set = Set::BOX;
			break;
		}

		if (spaces.size() < 4) //TODO: explain this
			continue;

		std::set<int>::iterator it1;

		for (it1 = spaces.begin(); it1 != spaces.end(); it1++)
		{
			std::set<int> candidates1 = boardMap.spaceCandidates[*it1];
			std::set<int> equivSpaces;
			equivSpaces.insert(*it1);

			std::set<int>::iterator it2 = it1;
			std::advance(it2, 1);
			for (it2; it2 != spaces.end(); it2++)
			{
				std::set<int> candidates2 = boardMap.spaceCandidates[*it2];

				if (candidates1 == candidates2)
				{
					equivSpaces.emplace(*it2);
				}
			}

			if (equivSpaces.size() == candidates1.size()) //TODO think about moving this to teh above for loop
			{
				std::set<int> affectedSpaces = getDifference(spaces, equivSpaces);
				if (boardMap.removeCandidates(candidates1, affectedSpaces))
				{
					step->updateNakedSubset(i / 9, set, candidates1, equivSpaces, affectedSpaces);
					return true;
				}
			}
		}
	}

	return false;
}