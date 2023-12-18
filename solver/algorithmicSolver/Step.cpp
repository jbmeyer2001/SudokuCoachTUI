#include <iostream>
#include <windows.h>

#include "Step.h"

void Step::clearStep(void)
{
	this->name = StepID::NA;
}

void Step::updateSoleCandidate(int row, int col, int val)
{
	this->name = StepID::SOLECANDIDATE;
	this->soleCandidate.row = row;
	this->soleCandidate.col = col;
	this->soleCandidate.val = val;
}

void Step::updateUniqueCandidate(int row, int col, int val, Set set)
{
	this->name = StepID::UNIQUECANDIDATE;
	this->uniqueCandidate.row = row;
	this->uniqueCandidate.col = col;
	this->uniqueCandidate.val = val;
	this->uniqueCandidate.rowColBox = set;
}

void Step::updateBlockRowCol(int val, int boxNum, int rowColNum, Set rowCol, BoxSubset boxSubset, std::set<int> affectedSpaces)
{
	this->name = StepID::BLOCKROWCOL;
	this->blockRowCol.val = val;
	this->blockRowCol.boxNum = boxNum;
	this->blockRowCol.rowColNum = rowColNum;
	this->blockRowCol.rowCol = rowCol;
	this->blockRowCol.affectedSpaces = affectedSpaces;
}

void Step::updateBlockBlock(int val, int box1, int box2, int box3, Set rowCol, BoxSubset boxSubset1, BoxSubset boxSubset2, BoxSubset boxSubset3, std::set<int> affectedSpaces)
{
	this->name = StepID::BLOCKBLOCK;
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
	this->name = StepID::NAKEDSUBSET;
	this->nakedSubset.rowColBoxNum = rowColBoxNum;
	this->nakedSubset.rowColBox = setToString(rowColBox);
	this->nakedSubset.vals = vals;
	this->nakedSubset.affectedSpaces = affectedSpaces;
}

void Step::updateHiddenSubset(int rowColBoxNum, Set rowColBox, std::set<int> affectedSpaces, std::set<int> vals)
{
	this->name = StepID::HIDDENSUBSET;
	this->hiddenSubset.rowColBoxNum = rowColBoxNum;
	this->hiddenSubset.rowColBox = setToString(rowColBox);
	this->hiddenSubset.vals = vals;
	this->hiddenSubset.affectedSpaces = affectedSpaces;
}

void Step::updateXWing(int val, int row1, int row2, int col1, int col2, Set rowCol, std::set<int> affectedSpaces)
{
	this->name = StepID::HIDDENSUBSET;
	this->xWing.val = val;
	this->xWing.row1 = row1;
	this->xWing.row2 = row2;
	this->xWing.col1 = col1;
	this->xWing.col2 = col2;
	this->xWing.rowCol = setToString(rowCol);
	this->xWing.affectedSpaces = affectedSpaces;
}

static void setColor(Color color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)color);
}

void Step::printSoleCandidate(int puzzle[9][9]) {
	SoleCandidate cur = this->soleCandidate;

	setColor(WHITE);
	printf("STEP: the value %d was placed in row %d column %d using the 'sole candidate' pattern.\n",
		cur.val,
		cur.row + 1,
		cur.col + 1);

	setColor(BLUE);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (i == cur.row && j == cur.col)
			{
				setColor(RED);
				std::cout << puzzle[i][j] << " ";
				setColor(BLUE);
			}
			else
			{
				std::cout << puzzle[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}

}

void Step::printUniqueCandidate(int puzzle[9][9]) {
	UniqueCandidate cur = this->uniqueCandidate;

	std::set<int> highlightSubset;
	int subset = 0;
	switch (cur.rowColBox)
	{
	case Set::ROW:
		subset = cur.row;
		highlightSubset = getRow(subset);
		break;
	case Set::COL:
		subset = cur.col;
		highlightSubset = getCol(subset);
		break;
	case Set::BOX:
		subset = (cur.row / 3) * 3 + cur.col / 3;
		highlightSubset = getBox(subset);
		break;
	default:
		std::cout << "**ERROR: we should NOT be here**" << std::endl;
	}

	setColor(WHITE);
	printf("STEP: the value %d was placed in row %d column %d using the 'unique candidate' pattern.\nThe space at row %d column %d was the only space within %s %d with %d as a candidate.\n",
		cur.val,
		cur.row + 1,
		cur.col + 1,
		cur.row + 1,
		cur.col + 1,
		setToString(cur.rowColBox).c_str(),
		subset + 1,
		cur.val);



	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{ 
			if (i == cur.row && j == cur.col) { 
				setColor(RED); //if it's the val set to red
			} 
			else if (highlightSubset.contains(i * 9 + j)) { 
				setColor(YELLOW); //if it's the subset that caused the unique candidate then yellow
			} 
			else {
				setColor(BLUE); //for all other values: blue
			} 

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Step::printBlockRowCol(int puzzle[9][9]) {
	BlockRowCol cur = this->blockRowCol;
	int subset;

	//the subset of all the spaces in either the row or the column specified by cur.rowColNum
	std::set<int> removeSubset = (cur.rowCol == Set::ROW) ? getRow(cur.rowColNum) : getCol(cur.rowColNum);

	//the subset of all the spaces in the box num specified by cur.boxNum
	std::set<int> boxSubset = getIntersection(getBox(cur.boxNum), removeSubset);

	//the subset of all values in eithe rthe row or column specified by cur.rowColNum, but NOT in the box specified by cur.boxNum
	removeSubset = getDifference(removeSubset, boxSubset);

	setColor(WHITE);
	printf("the value %d was removed as a candidate from all the spaces in %s %d except in box %d.\nThis is because there was a block to column/row interaction.\nThe only places a %d could go within box %d was in %s %d, so we could remove it as a candidate from the rest of %s %d.\n",
		cur.val,
		setToString(cur.rowCol).c_str(),
		cur.rowColNum + 1,
		cur.boxNum + 1,
		cur.val,
		cur.boxNum + 1,
		setToString(cur.rowCol).c_str(),
		cur.rowColNum + 1,
		setToString(cur.rowCol).c_str(),
		cur.rowColNum + 1);
	
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (boxSubset.contains(i * 9 + j)) {
				setColor(GREEN);
			}
			else if (removeSubset.contains(i * 9 + j)) {
				setColor(YELLOW);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Step::printBlockBlock(int puzzle[9][9]) {
	printf("block block NOT IMPLEMENTD:\n");
}

void Step::printNakedSubset(int puzzle[9][9]) {
	printf("naked subset NOT IMPLEMENTD:\n");
}

void Step::printHiddenSubset(int puzzle[9][9]) {
	printf("hidden subset NOT IMPLEMENTD:\n");
}

void Step::printXWing(int puzzle[9][9]) {
	printf("x wing NOT IMPLEMENTD:\n");
}

void Step::printStep(int puzzle[9][9])
{
	switch (name) {

	case StepID::SOLECANDIDATE:
		printSoleCandidate(puzzle);
		break;
	case StepID::UNIQUECANDIDATE:
		printUniqueCandidate(puzzle);
		break;
	case StepID::BLOCKROWCOL:
		printBlockRowCol(puzzle);
		break;
	case StepID::BLOCKBLOCK:
		printBlockBlock(puzzle);
		break;
	case StepID::NAKEDSUBSET:
		printNakedSubset(puzzle);
		break;
	case StepID::HIDDENSUBSET:
		printHiddenSubset(puzzle);
		break;
	case StepID::XWING:
		printXWing(puzzle);
		break;
	default:
		std::cout << "**ERROR: we should NOT be here**" << std::endl;
	}
}