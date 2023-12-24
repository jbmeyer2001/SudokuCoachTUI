#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::nakedSubset(void)
{
	Set set;
	std::set<int> spaces;

	//iterate through all subsets (all 9 rows/columns/boxes)
	for (int i = 0; i < 27; i++) {
		switch (i % 3) {
		case 0:
			spaces = boardMap.getUnfilledSpaces(getRow(i / 3));
			set = Set::ROW;
			break;
		case 1:
			spaces = boardMap.getUnfilledSpaces(getCol(i / 3));
			set = Set::COL;
			break;
		case 2:
			spaces = boardMap.getUnfilledSpaces(getBox(i / 3));
			set = Set::BOX;
			break;
		}

		//a naked subset cannot occur within a subset less than 4 spaces
		//if it did, the interaction would already have been covered by unique candidate
		if (spaces.size() < 4) {
			continue;
		}

		std::set<int>::iterator it1;

		//iterate through all spaces in the given subset
		for (it1 = spaces.begin(); it1 != spaces.end(); it1++) {
			std::set<int> candidates1 = boardMap.spaceCandidates[*it1];
			std::set<int> equivSpaces;
			equivSpaces.insert(*it1);

			//iterate through all remaining spaces to see how many other spaces have exactly the same candidates
			std::set<int>::iterator it2 = it1;
			std::advance(it2, 1);
			for (it2; it2 != spaces.end(); it2++) {
				std::set<int> candidates2 = boardMap.spaceCandidates[*it2];

				if (candidates1 == candidates2)
				{
					equivSpaces.emplace(*it2);
				}
			}

			//if the number of spaces that have the exact same candidates is equivalent
			//to the number of candidates, that is a naked subset and those candidates may
			//be removed from the other spaces within the given row/column/box
			if (equivSpaces.size() == candidates1.size()) {
				std::set<int> affectedSpaces = getDifference(spaces, equivSpaces);
				if (boardMap.removeCandidates(candidates1, affectedSpaces)) {
					step.updateNakedSubset(i / 3, set, candidates1, equivSpaces, affectedSpaces);
					return true;
				}
			}
		}
	}

	return false;
}