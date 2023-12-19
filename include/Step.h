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
rowColBox		- a string indicating whether stepSubsetNum1 was a row, a column, or a box
*/
struct UniqueCandidate {
	int val = -1;
	int row = -1;
	int col = -1;
	Set rowColBox = Set::NA;
};

/*
Val				- indicate which value was removed form affected spaces
boxNum			- the box in which the interaction occured
rowColNum		- the value (0-8) of that row or column
rowCol			- whether the interaction was on a row or column
AffectedSpaces	- the an std::set of the spaces (0-81) that were affected
*/
struct BlockRowCol {
	int val = -1;
	int boxNum = -1;
	int rowColNum = -1;
	Set rowCol = Set::NA;
};

/*
TODO:
*/
struct BlockBlock {
	int val = -1;
	int affectedBox = -1;
	int affectingSubset1 = -1;
	int affectingSubset2 = -1;
	int affectingBox1 = -1;
	int affectingBox2 = -1;
	Set rowCol = Set::NA;
};

/*
rowColBox		- an indication of whether the interaction occured within a row, column, or a box
rowColBoxNum	- an indication of the row/col/box num in which the interaction occured
vals			- the val's that were able to removed as candidates from the affected spaces
affectingSpaces - the spaces that contain all the vals which are causing the interaction
affectedSpaces	- the spaces who had their candidates removed as a result of the interaction
*/
struct NakedSubset {
	int rowColBoxNum = -1;
	Set rowColBox = Set::NA;
	std::set<int> vals;
	std::set<int> affectingSpaces;
	std::set<int> affectedSpaces;
};

/*
rowColBox		- an indication of whether the interaction occured within a row, column, or a box
rowColBoxNum	- an indication of the row/col/box num in which the interaction occured
vals			- the val's that were able to removed as candidates from the affected spaces
affectingSpaces - the spaces that contain all the vals which are causing the interaction
affectedSpaces	- the spaces who had their candidates removed as a result of the interaction
*/
struct HiddenSubset {
	int rowColBoxNum = -1;
	Set rowColBox = Set::NA;
	std::set<int> removalVals;
	std::set<int> subsetVals;
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
	Set rowCol = Set::NA;
	std::set<int> affectedSpaces;
};

class Step {
private:
	StepID name;
	struct SoleCandidate soleCandidate;
	struct UniqueCandidate uniqueCandidate;
	struct BlockRowCol blockRowCol;
	struct BlockBlock blockBlock;
	struct NakedSubset nakedSubset;
	struct HiddenSubset hiddenSubset;
	struct XWing xWing;

	void printSoleCandidate(int puzzle[9][9]);
	void printUniqueCandidate(int puzzle[9][9]);
	void printBlockRowCol(int puzzle[9][9]);
	void printBlockBlock(int puzzle[9][9]);
	void printNakedSubset(int puzzle[9][9]);
	void printHiddenSubset(int puzzle[9][9]);
	void printXWing(int puzzle[9][9]);
public:
	void clearStep(void);
	void updateSoleCandidate(int row, int col, int val);
	void updateUniqueCandidate(int row, int col, int val, Set set);
	void updateBlockRowCol(int val, int boxNum, int rowColNum, Set rowCol);
	void updateBlockBlock(int val, int affectedBox, int subset1, int subset2, int box1, int box2, Set rowCol);
	void updateNakedSubset(int rowColBoxNum, Set rowColBox, std::set<int> vals, std::set<int> affectingSpaces, std::set<int> affectedSpaces);
	void updateHiddenSubset(int rowColBoxNum, Set rowColBox, std::set<int> removalVals, std::set<int> susbsetVals, std::set<int> affectedSpaces);
	void updateXWing(int val, int row1, int row2, int col1, int col2, Set rowCol, std::set<int> affectedSpaces);
	void solved(void) { this->name = StepID::SOLVED; }
	void cantSolve(void) { this->name = StepID::CANTSOLVE;  }
	void unsolveable(void) { this->name = StepID::UNSOLVEABLE; }

	StepID getStepID(void) { return this->name; }
	void printStep(int puzzle[9][9]);
};