#pragma once

#include <set>
#include <vector>
#include <string>

#include "BoardMap.h"
#include "Utility.h"
#include "StepInfo.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;
	StepInfo* stepInfo;

	bool soleCandidate(void);
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set set);
	bool blockColRowInteraction(void);
	bool blockBlockInteraction(void);
	bool nakedSubset(void);
	bool hiddenSubset(void);
	std::set<int> getNextPartition(std::vector<int> spaces, unsigned short& prev);
	bool XWing(void);
	bool XWingHelper(std::set<int> rows, std::set<int> cols, std::set<int> intersections);

	

	

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void nextStep(void);
	void solve(void);
};

