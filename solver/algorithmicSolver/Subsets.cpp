#include "AlgorithmicSolver.h"

bool AlgorithmicSolver::subsets(void) {
	Set set;
	std::set<int> spaces;

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

		//neither naked nor hidden subset can occur within a subset less than 4 spaces
		//if it did, the interaction would already have been covered by unique candidate
		if (spaces.size() < 4) {
			continue;
		}

		unsigned short prev = (unsigned short)2;
		std::vector<int> spacesVec(spaces.begin(), spaces.end());
		std::set<int> partition = getNextPartition(spacesVec, prev);
		while (!partition.empty()) {
			std::set<int> other = getDifference(spaces, partition);
			std::set<int> partitionCandidates = boardMap.getCandidates(partition);
			std::set<int> otherCandidates = boardMap.getCandidates(other);

			/*
			* Naked Subset
			* if the number of candidates in a given paritition (of a row/column/box) is equal to the size of that subset, 
			* those candidates cannot exist in the candidates not within that partition.
			*/
			if (partitionCandidates.size() == partition.size()) {
				if (boardMap.removeCandidates(partitionCandidates, other)) {
					step.updateNakedSubset(i / 3, set, partitionCandidates, partition, other);
					return true;
				}
			}

			/*
			* Hidden Subset
			* If a number of candidates equal to the size of a partition ONLY exist within that partition, then 
			* those partition spaces cannot be any other candidate.
			*/
			if (getDifference(partitionCandidates, otherCandidates).size() == partition.size()) {
				int unused = 0;
				if (boardMap.removeCandidates(otherCandidates, partition)) {
					step.updateHiddenSubset(i / 3, set, getDifference(partitionCandidates, otherCandidates), partition);
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

	//if we've gotten larger than the size of spaces, return an empty set indicating we're done
	if (curInt >= pow(2, spaces.size()) - 1) {
		return retval;
	}

	//bit mask for generating partition
	for (uint16_t i = 0; i < spaces.size(); i++) {
		uint16_t mask = pow(2, i);

		if ((prev & mask) == mask) {
			retval.insert(spaces[i]);
		}
	}

	//if the size of this partition (or other) would be equal to 1, it is
	//not possible for there to be a hidden candidate
	if (retval.size() < 2 || retval.size() > (spaces.size() - 2)) {
		retval.clear();
		prev = cur;
		goto getPartition;
	}

	prev = cur;
	return retval;
}