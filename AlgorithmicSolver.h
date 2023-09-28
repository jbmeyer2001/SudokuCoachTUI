#pragma once

#include <set>

#include "BoardMap.h"
#include "Utility.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;
	enum class Interaction
	{
		BLOCKROWCOL = 0,
		BLOCKBLOCK = 1,
		NAKEDSUBSET = 2,
		HIDDENSUBSET = 3,
	};

	bool soleCandidate(void);
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces);
	bool blockColRowInteraction(void);
	std::set<int> getCommonalities(int i1, int i2, int i3);
	std::set<int> getCandidates(std::set<int> spaces);
	bool blockBlockInteraction(void);
	std::set<int> getUnfilledSpaces(std::set<int> spaces);
	bool checkSubsets(void);
	bool nakedSubset(std::set<int> spaces, Set s);
	bool hiddenSubset(std::set<int> spaces, Set s);
	bool removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces, Interaction interaction);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void solve(void);

};

