#pragma once

#include <set>

#include "BoardMap.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	std::set<int> unfilled;
	BoardMap* boardMap;

	bool soleCandidate(void);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void solve(void);

};

