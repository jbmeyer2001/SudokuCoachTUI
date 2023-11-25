#pragma once

#include <set>
#include <vector>

#include "BoardMap.h"
#include "Utility.h"
#include "Step.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;
	Step* step;

	bool soleCandidate(void);
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set s);
	bool blockColRowInteraction(void);
	bool blockBlockInteraction(void);
	bool nakedSubset(void);
	bool hiddenSubset(void);
	std::set<int> getNextPartition(std::vector<int> spaces, unsigned short& prev);
	bool XWing(void);
	bool XWingHelper(std::set<int> rowSpaces, std::set<int> colSpaces, std::set<int> intSpaces, int row1, int row2, int col1, int col2);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void nextStep(void);
	void solve(void);
};

