#pragma once

#include <string>
#include <set>

#include "Utility.h"

/*
val				- the value (1-9) placed by the most recent call to sole candidate
col				- the value (0-8) of the column that the value was placed in
row				- the value (0-8) of the row that the value was placed in
*/
struct SoleCandidate {
	int val = -1;
	int row = -1;
	int col = -1;
};

/*
val				- the value (1-9) placed by the most recent call to unique candidate
col				- the value (0-8) of the column that the value was placed in
row				- the value (0-8) of the row that the value was placed in
rowColBoxNum	- the value of the row/col/box in which puzzle['row', 'col'] was the only space that could be 'value'
rowColBox		- a string indicating whether stepSubsetNum1 was a row, a column, or a box
*/
struct UniqueCandidate {
	int val = -1;
	int row = -1;
	int col = -1;
	int rowColBoxNum = -1;
	std::string rowColBox = "";
};

/*
Val				- indicate which value was removed form affected spaces
boxNum			- the box in which the interaction occured
rowColNum		- the value (0-8) of that row or column
boxSubset		- where (top/mid/bot/left/right) in the box the interaction occured
rowCol			- whether the interaction was on a row or column
AffectedSpaces	- the an std::set of the spaces (0-81) that were affected
*/
struct BlockRowCol {
	int val = -1;
	int boxNum = -1;
	int rowColNum = -1;
	std::string boxSubset = "";
	std::string rowCol = "";
	std::set<int> affectedSpaces;
};

/*
val				- the value to be removed as candidates from affected spaces
box1			- the box containing candidates that must be removed
box2			- the first box causing the interaction
box2			- the second box causing the interaction
rowCol			- whether the interacton occured in a row or a column
boxSubset1		- where (top/mid/bot/left/right) in the box the first part of the interaction occured
boxSubset2		- where (top/mid/bot/left/right) in the box the second part of the interaction occured
boxSubset3		- where (top/mid/bot/left/right) in the box the third part of the interaction occured
affectedSpaces	- the spaces to have their candidates removed
*/
struct BlockBlock {
	int val = -1;
	int box1 = -1;
	int box2 = -1;
	int box3 = -1;
	std::string rowCol = "";
	std::string boxSubset1 = "";
	std::string boxSubset2 = "";
	std::string boxSubset3 = "";
	std::set<int> affectedSpaces;
};

//TODO: add documentation for what this means in naked/hidden subset
struct NakedSubset {
	int rowColBoxNum = -1;
	std::string rowColBox = "";
	std::set<int> vals;
	std::set<int> affectedSpaces;
};

struct HiddenSubset {
	int rowColBoxNum = -1;
	std::string rowColBox = "";
	std::set<int> vals;
	std::set<int> affectedSpaces;
};

/*
val				- the value of the candidate that was removed from the affected spaces
row1			- to indicate one of the rows used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
row2			- to indicate the other row used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
col1			- to indicate on eof the cols used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
col2			- to indicate the other colused in the x wign (not updated in this function, but updated by the time xwing finishes execution)
rowCol				- to indicate whether the columns or the rows had some of their candidates removed
affectedSpaces	- the spaces that had val removed from their candidates
*/
struct XWing {
	int val = -1;
	int row1 = -1;
	int row2 = -1;
	int col1 = -1;
	int col2 = -1;
	std::string rowCol = "";
	std::set<int> affectedSpaces;
};

class Step {
private:
	std::string name;
	struct SoleCandidate soleCandidate;
	struct UniqueCandidate uniqueCandidate;
	struct BlockRowCol blockRowCol;
	struct BlockBlock blockBlock;
	struct NakedSubset nakedSubset;
	struct HiddenSubset hiddenSubset;
	struct XWing xWing;

	void clearStep(void);
	void updateSoleCandidate(int row, int col, int val);
	void updateUniqueCandidate(int row, int col, int val, int rowColBoxNum, Set set);
	void updateBlockRowCol(int val, int boxNum, int rowColNum, Set rowCol, BoxSubset boxSubset, std::set<int> affectedSpaces);
	void updateBlockBlock(int val, int box1, int box2, int box3, Set rowCol, BoxSubset boxSubset1, BoxSubset boxSubset2, BoxSubset boxSubset3, std::set<int> affectedSpaces);
	void updateNakedSubset(int rowColBoxNum, Set rowColBox, std::set<int> affectedSpaces, std::set<int> vals);
	void updateHiddenSubset(int rowColBoxNum, Set rowColBox, std::set<int> affectedSpaces, std::set<int> vals);
	void updateXWing(int val, int row1, int row2, int col1, int col2, Set rowCol, std::set<int> affectedSpaces);

public:
	//getters
	std::string getStep(void) { return this->name; }
	SoleCandidate getSoleCandidate(void) { return soleCandidate; }
	UniqueCandidate getUniqueCandidate(void) { return uniqueCandidate; }
	BlockRowCol getBlockRowCol(void) { return blockRowCol; }
	BlockBlock getBlockBlock(void) { return blockBlock; }
	NakedSubset getNakedSubset(void) { return nakedSubset; }
	HiddenSubset getHiddenSubset(void) { return hiddenSubset; }
	XWing getXWing(void) { return xWing; }

	friend class AlgorithmicSolver;
};