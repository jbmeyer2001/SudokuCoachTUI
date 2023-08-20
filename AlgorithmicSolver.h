#pragma once

#include <set>


class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	std::set<int> unfilled;
	BoardMap boardMap;

	void soleCandidate(void);

public:
	AlgorithmicSolver(int puzzle[9][9]);
};

