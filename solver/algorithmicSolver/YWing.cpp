#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::YWing(void) {\

	//generate a set of all the spaces with only two candidates
	std::set<int> twoCandSpaces;
	for (int i = 0; i < 81; i++) {
		if (boardMap.getCandidates(i).size() == 2) {
			twoCandSpaces.emplace(i);
		}
	}

	//iterate through those to find if they are the 'base' of the ywing
	std::set<int>::iterator it;
	for (it = twoCandSpaces.begin(); it != twoCandSpaces.end(); it++) {
		int cur = *it;
		int row = cur / 9;
		int col = cur % 9;
		int box = (cur / 27) * 3 + (cur % 9) / 3;
		std::set<int> curCand = boardMap.getCandidates(cur);

		//generate set of row, column, and box sharings (spaces within twoCandSpaces that share a row/col/box with the current space)
		std::set<int> curAffectingSpaces = getUnion(getRow(row), getCol(col), getBox(box));
		std::set<int> shareRowColBox = getIntersection(
			twoCandSpaces, 
			curAffectingSpaces
		);
		shareRowColBox.erase(cur);

		//remove the spaces from share(Row/Col/Box) whose intersection with the current space isn't 1
		std::set<int> keep;
		std::set<int>::iterator share;
		for (share = shareRowColBox.begin(); share != shareRowColBox.end(); share++) {
			int space = *share;
			if (getIntersection(curCand, boardMap.getCandidates(space)).size() == 1) {
				keep.emplace(space);
			}
		}
		shareRowColBox = keep;
		keep.clear();

		//stop iterating if everything is empty
		if (shareRowColBox.empty()) {
			continue;
		}

		//check each combination of row/col row/box and col/box spaces to see if their union with cur is 3
		//but their intersection with one another is 1, as this would indicate y-wing pattern.
		std::set<int>::iterator ycheck1;
		std::set<int>::iterator ycheck2;

		for (ycheck1 = shareRowColBox.begin(); ycheck1 != shareRowColBox.end(); ycheck1++) {
			for (ycheck2 = shareRowColBox.begin(); ycheck2 != shareRowColBox.end(); ycheck2++) {
				int wing1 = *ycheck1;
				int wing2 = *ycheck2;

				int wing1row = wing1 / 9;
				int wing1col = wing1 % 9;
				int wing1box = (wing1 / 27) * 3 + (wing1 % 9) / 3;

				int wing2row = wing2 / 9;
				int wing2col = wing2 % 9;
				int wing2box = (wing2 / 27) * 3 + (wing2 % 9) / 3;

				std::set<int> affectedSpaces = getIntersection(
					getUnion(getRow(wing1row), getCol(wing1col), getBox(wing1box)), 
					getUnion(getRow(wing2row), getCol(wing2col), getBox(wing2box))
				);
				//we don't want spaces that share a row and or a column and or a box with each other (only with cur)
				if (affectedSpaces.size() >= 9) {
					continue;
				}
				affectedSpaces = getDifference(affectedSpaces, curAffectingSpaces);

				std::set<int> wing1Cand = boardMap.getCandidates(wing1);
				std::set<int> wing2Cand = boardMap.getCandidates(wing2);
				//in order for the interaction to work, the 'wing' spaces must share a candidate that the 'base' does not have
				if (getIntersection(wing1Cand, wing2Cand).size() != 1) {
					continue;
				}
				if (getUnion(wing1Cand, wing2Cand, curCand).size() != 3) {
					continue;
				}

				//if we've made it here, it means we've found a y-wing.
				//the y-wing is only meaningful though if we can remove candidates because of it.
				int candidate = *getIntersection(wing1Cand, wing2Cand).begin();
				if (boardMap.removeCandidates(candidate, affectedSpaces)) {
					step.updateYWing(candidate, cur, wing1, wing2, affectedSpaces);
					return true;
				}
			}
		}
	}
}