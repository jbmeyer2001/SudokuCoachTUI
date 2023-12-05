#include "Step.h"

void Step::clearStep(void)
{
	this->name = "NOSTEP";
}

void Step::updateSoleCandidate(int row, int col, int val)
{
	this->name = "SOLECANDIDATE";
	this->soleCandidate.row = row;
	this->soleCandidate.col = col;
	this->soleCandidate.val = val;
}

void Step::updateUniqueCandidate(int row, int col, int val, int rowColBoxNum, Set set)
{
	this->name = "UNIQUECANDIDATE";
	this->uniqueCandidate.row = row;
	this->uniqueCandidate.col = col;
	this->uniqueCandidate.val = val;
	this->uniqueCandidate.rowColBoxNum = rowColBoxNum;
	this->uniqueCandidate.rowColBox = setToString(set);
}

void Step::updateBlockRowCol(int val, int boxNum, int rowColNum, Set rowCol, BoxSubset boxSubset, std::set<int> affectedSpaces)
{
	this->name = "BLOCKROWCOL";
	this->blockRowCol.val = val;
	this->blockRowCol.boxNum = boxNum;
	this->blockRowCol.rowColNum = rowColNum;
	this->blockRowCol.rowCol = setToString(rowCol);
	this->blockRowCol.boxSubset = boxSubsetToString(boxSubset);
	this->blockRowCol.affectedSpaces = affectedSpaces;
}

void Step::updateBlockBlock(int val, int box1, int box2, int box3, Set rowCol, BoxSubset boxSubset1, BoxSubset boxSubset2, BoxSubset boxSubset3, std::set<int> affectedSpaces)
{
	this->name = "BLOCKBLOCK";
	this->blockBlock.val = val;
	this->blockBlock.box1 = box1;
	this->blockBlock.box2 = box2;
	this->blockBlock.box3 = box3;
	this->blockBlock.rowCol = setToString(rowCol);
	this->blockBlock.boxSubset1 = boxSubsetToString(boxSubset1);
	this->blockBlock.boxSubset2 = boxSubsetToString(boxSubset2);
	this->blockBlock.boxSubset3 = boxSubsetToString(boxSubset3);
	this->blockBlock.affectedSpaces = affectedSpaces;
}

void Step::updateNakedSubset(int rowColBoxNum, Set rowColBox, std::set<int> affectedSpaces, std::set<int> vals)
{
	this->name = "NAKEDSUBSET";
	this->nakedSubset.rowColBoxNum = rowColBoxNum;
	this->nakedSubset.rowColBox = setToString(rowColBox);
	this->nakedSubset.vals = vals;
	this->nakedSubset.affectedSpaces = affectedSpaces;
}

void Step::updateHiddenSubset(int rowColBoxNum, Set rowColBox, std::set<int> affectedSpaces, std::set<int> vals)
{
	this->name = "HIDDENSUBSET";
	this->hiddenSubset.rowColBoxNum = rowColBoxNum;
	this->hiddenSubset.rowColBox = setToString(rowColBox);
	this->hiddenSubset.vals = vals;
	this->hiddenSubset.affectedSpaces = affectedSpaces;
}

void Step::updateXWing(int val, int row1, int row2, int col1, int col2, Set rowCol, std::set<int> affectedSpaces)
{
	this->name = "XWING";
	this->xWing.val = val;
	this->xWing.row1 = row1;
	this->xWing.row2 = row2;
	this->xWing.col1 = col1;
	this->xWing.col2 = col2;
	this->xWing.rowCol = setToString(rowCol);
	this->xWing.affectedSpaces = affectedSpaces;
}