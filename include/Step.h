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
	int val;
	int row;
	int col;
};

/*
val				- the value (1-9) placed by the most recent call to unique candidate
col				- the value (0-8) of the column that the value was placed in
row				- the value (0-8) of the row that the value was placed in
rowColBoxNum	- the value of the row/col/box in which puzzle['row', 'col'] was the only space that could be 'value'
rowColBox		- a string indicating whether stepSubsetNum1 was a row, a column, or a box
*/
struct UniqueCandidate {
	int val;
	int row;
	int col;
	int rowColBoxNum;
	std::string rowColBox;
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
	int val;
	int boxNum;
	int rowColNum;
	std::string boxSubset;
	std::string rowCol;
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
	int val;
	int box1;
	int box2;
	int box3;
	std::string rowCol;
	std::string boxSubset1;
	std::string boxSubset2;
	std::string boxSubset3;
	std::set<int> affectedSpaces;
};

//TODO: add documentation for what this means in naked/hidden subset
struct NakedSubset {
	int rowColBoxNum;
	std::string rowColBox;
	std::set<int> vals;
	std::set<int> affectedSpaces;
};

struct HiddenSubset {
	int rowColBoxNum;
	std::string rowColBox;
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
	int val;
	int row1;
	int row2;
	int col1;
	int col2;
	std::string rowCol;
	std::set<int> affectedSpaces;
};

class Step {
private:
	std::string step;
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
	//TODO: remove the comment just below this lol
	//called by class_::def_property_readonly()

	//step getter
	std::string getStep(void) { return this->step; }

	//sole candidate getters
	int getSoleCandidateRow(void) { return soleCandidate.row; }
	int getSoleCandidateCol(void) { return soleCandidate.col; }
	int getSoleCandidateVal(void) { return soleCandidate.val; }

	//unique candidate getters
	int getUniqueCandidateRow(void) { return uniqueCandidate.row; }
	int getUniqueCandidateCol(void) { return uniqueCandidate.col; }
	int getUniqueCandidateVal(void) { return uniqueCandidate.val; }
	int getUniqueCandidateRowColBoxNum(void) { return uniqueCandidate.rowColBoxNum; }
	std::string getUniqueCandidateRowColBox(void) { return uniqueCandidate.rowColBox; }

	//block row col getters
	int getBlockRowColVal(void) { return blockRowCol.val; }
	int getBlockRowColBoxNum(void) { return blockRowCol.boxNum; }
	int getBlockRowColRowColNum(void) { return blockRowCol.rowColNum; }
	std::string getBlockRowColBoxSubset(void) { return blockRowCol.boxSubset; }
	std::set<int> getBlockRowColAffectedSpaces(void) { return blockRowCol.affectedSpaces; }
	
	//block block getters
	int getBlockBlockVal(void) { return blockBlock.val; }
	int getBlockBlockBox1(void) { return blockBlock.box1; }
	int getBlockBlockBox2(void) { return blockBlock.box2; }
	int getBlockBlockBox3(void) { return blockBlock.box3; }
	std::string getBlockBlockRowCol(void) { return blockBlock.rowCol; }
	std::string getBlockBlockBoxSubset1(void) { return blockBlock.boxSubset1; }
	std::string getBlockBlockBoxSubset2(void) { return blockBlock.boxSubset2; }
	std::string getBlockBlockBoxSubset3(void) { return blockBlock.boxSubset3; }
	std::set<int> getBlockBlockAffectedSpaces(void) { return blockBlock.affectedSpaces; }

	//naked subset getters
	int getNakedSubsetRowColBoxNum(void) { return nakedSubset.rowColBoxNum; }
	std::string getNakedSubsetRowColBox(void) { return nakedSubset.rowColBox; }
	std::set<int> getNakedSubsetVals(void) { return nakedSubset.vals; }
	std::set<int> getNakedSubsetAffectedSpaces(void) { return nakedSubset.affectedSpaces; }

	//hidden subset getters
	int getHiddenSubsetRowColBoxNum(void) { return hiddenSubset.rowColBoxNum; }
	std::string getHiddenSubsetRowColBox(void) { return hiddenSubset.rowColBox; }
	std::set<int> getHiddenSubsetVals(void) { return hiddenSubset.vals; }
	std::set<int> getHiddenSubsetAffectedSpaces(void) { return hiddenSubset.affectedSpaces; }

	//X wing getters
	int getXWingVal(void) { return xWing.val; }
	int getXWingRow1(void) { return xWing.row1; }
	int getXWingRow2(void) { return xWing.row2; }
	int getXWingCol1(void) { return xWing.col1; }
	int getXWingCol2(void) { return xWing.col2; }
	std::string getXWingRowCol(void) { return xWing.rowCol; }
	std::set<int> getXWingAffectedSpaces(void) { return xWing.affectedSpaces; }

	friend class AlgorithmicSolver;
};