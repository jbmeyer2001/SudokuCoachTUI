#pragma once

#include <set>

class BoardMap
{
private:
	std::set<int> unfilled;
	std::set<int> spaceCandidates[81];

	//utility functions for use in algorithmic solver
	std::set<int> getCandidates(std::set<int> spaces);
	std::set<int> getCandidates(int space);
	bool removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces, int& value);
	bool removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces);
	bool removeCandidates(int candidate, std::set<int> affectedSpaces);
	std::set<int> getUnfilledSpaces(std::set<int> spaces);
	std::set<int> getCommonalities(int i1, int i2, int i3);

public:
	void init(int puzzle[9][9]);
	void insert(int spaceNum, int val);

	friend class AlgorithmicSolver;
};

