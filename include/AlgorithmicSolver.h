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
	int puzzleStart[9][9] = { -1 };
	BoardMap boardMap;
	Step *step;

	bool solved(void);

	//check to see if we can identify a sole candidate
	bool soleCandidate(void);

	//check to see if we can identify a unique candidate
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces, Set s);

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

	void checkIfSolveable(void);

public:
	AlgorithmicSolver(int puzzle[9][9], Step& step);

	void nextStep(void);
	StepID check(void);
	void printStep(void);
};

