#pragma once

#include <set>

class BoardMap
{
private:
	std::set<int> unfilled;
	std::set<int> spaceCandidates[81];

public:
	BoardMap(int puzzle[9][9]);
	void insert(int spaceNum, int val);

	friend class AlgorithmicSolver;
};

