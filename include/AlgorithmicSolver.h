#pragma once

#include <set>
#include <vector>
#include <string>

#include "BoardMap.h"
#include "Utility.h"
#include "Step.h"


class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;
	Step* step;

	//check to see if we can identify a sole candidate
	bool soleCandidate(void);

	//check to see if we can identify a unique candidate
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set s);

	//check to see if we can identify a block-col/row interaction
	bool blockColRowInteraction(void);

	//check to see if we can identify a block-bock
	bool blockBlockInteraction(void);

	//check to see if we can identify a naked subset
	bool nakedSubset(void);

	//check to see if we can identify a hidden subset
	bool hiddenSubset(void);
	std::set<int> getNextPartition(std::vector<int> spaces, unsigned short& prev);

	//check to see if we can identify an xwing
	bool XWing(void);
	bool XWingHelper(std::set<int> rowSpaces, std::set<int> colSpaces, std::set<int> intSpaces, int row1, int row2, int col1, int col2);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void nextStep(void);
	void solve(void);
};

