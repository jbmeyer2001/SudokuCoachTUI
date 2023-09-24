#pragma once

#include <set>

#include "BoardMap.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;

	bool soleCandidate(void);
	bool uniqueCandidate(void);
	std::set<int> uniqueCandidateHelper(int space, std::set<int> spaces);
	bool blockColRowInteraction(void);
	std::set<int> getCommonalities(int i1, int i2, int i3);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void solve(void);

};

