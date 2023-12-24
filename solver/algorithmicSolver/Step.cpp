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

void Step::updateBlockRowCol(int val, int boxNum, int rowColNum, Set rowCol)
{
	this->name = StepID::BLOCKROWCOL;
	this->blockRowCol.val = val;
	this->blockRowCol.boxNum = boxNum;
	this->blockRowCol.rowColNum = rowColNum;
	this->blockRowCol.rowCol = rowCol;
}

void Step::updateBlockBlock(int val, int affectedBox, int subset1, int subset2, int box1, int box2, Set rowCol)
{
	this->name = StepID::BLOCKBLOCK;
	this->blockBlock.val = val;
	this->blockBlock.affectedBox = affectedBox;
	this->blockBlock.affectingSubset1 = subset1;
	this->blockBlock.affectingSubset2 = subset2;
	this->blockBlock.affectingBox1 = box1;
	this->blockBlock.affectingBox2 = box2;
	this->blockBlock.rowCol = rowCol;
}

void Step::updateNakedSubset(int rowColBoxNum, Set rowColBox, std::set<int> vals, std::set<int> affectingSpaces, std::set<int> affectedSpaces)
{
	this->name = StepID::NAKEDSUBSET;
	this->nakedSubset.rowColBoxNum = rowColBoxNum;
	this->nakedSubset.rowColBox = rowColBox;
	this->nakedSubset.vals = vals;
	this->nakedSubset.affectingSpaces = affectingSpaces;
	this->nakedSubset.affectedSpaces = affectedSpaces;
}

void Step::updateHiddenSubset(int rowColBoxNum, Set rowColBox, std::set<int> subsetVals, std::set<int> affectedSpaces)
{
	this->name = StepID::HIDDENSUBSET;
	this->hiddenSubset.rowColBoxNum = rowColBoxNum;
	this->hiddenSubset.rowColBox = rowColBox;
	this->hiddenSubset.subsetVals = subsetVals;
	this->hiddenSubset.affectedSpaces = affectedSpaces;
}

void Step::updateXWing(int val, int row1, int row2, int col1, int col2, Set rowCol, std::set<int> affectedSpaces)
{
	this->name = StepID::XWING;
	this->xWing.val = val;
	this->xWing.row1 = row1;
	this->xWing.row2 = row2;
	this->xWing.col1 = col1;
	this->xWing.col2 = col2;
	this->xWing.rowCol = rowCol;
	this->xWing.affectedSpaces = affectedSpaces;
}

void Step::updateYWing(int candidate, int base, int wing1, int wing2, std::set<int> affectedSpaces) 
{
	this->name = StepID::YWING;
	this->yWing.candidate = candidate;
	this->yWing.base = base;
	this->yWing.wing1 = wing1;
	this->yWing.wing2 = wing2;
	this->yWing.affectedSpaces = affectedSpaces;
}

static void setColor(Color color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)color);
}

void Step::printSoleCandidate(int puzzle[9][9]) 
{
	SoleCandidate cur = this->soleCandidate;

	printf("STEP:sole candidate\nthe value %d was placed in row %d column %d using the 'sole candidate' pattern.\n",
		cur.val,
		cur.row + 1,
		cur.col + 1);

	setColor(BLUE);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (i == cur.row && j == cur.col)
			{
				setColor(MAGENTA);
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
	setColor(WHITE);
}

void Step::printUniqueCandidate(int puzzle[9][9]) 
{
	UniqueCandidate cur = this->uniqueCandidate;

	std::set<int> highlightSubset;
	int subset = 0;
	switch (cur.rowColBox) {
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

	printf("STEP:unique candidate\nthe value %d was placed in row %d column %d using the 'unique candidate' pattern.\nThe space at row %d column %d was the only space within %s %d with %d as a candidate.\n",
		cur.val,
		cur.row + 1,
		cur.col + 1,
		cur.row + 1,
		cur.col + 1,
		setToString(cur.rowColBox).c_str(),
		subset + 1,
		cur.val);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) { 
			if (i == cur.row && j == cur.col) { 
				setColor(MAGENTA); //if it's the val set to red
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
	setColor(WHITE);
}

void Step::printBlockRowCol(int puzzle[9][9]) 
{
	BlockRowCol cur = this->blockRowCol;

	//the subset of all the spaces in either the row or the column specified by cur.rowColNum
	std::set<int> removeSubset = (cur.rowCol == Set::ROW) ? getRow(cur.rowColNum) : getCol(cur.rowColNum);

	//the subset of all the spaces in the box num specified by cur.boxNum
	std::set<int> boxSubset = getIntersection(getBox(cur.boxNum), removeSubset);

	printf("STEP:block to column or row interaction\nthe value %d was removed as a candidate from all the spaces in %s %d except in box %d.\nThis is because there was a block to column/row interaction.\nThe only places a %d could go within box %d was in %s %d, so we could remove it as a candidate from the rest of %s %d.\n",
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
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (boxSubset.contains(i * 9 + j)) {
				setColor(YELLOW);
			}
			else if (removeSubset.contains(i * 9 + j)) {
				setColor(RED);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
}

void Step::printBlockBlock(int puzzle[9][9]) 
{
	BlockBlock cur = this->blockBlock;

	//the spaces within either the rows or columns in which the interaction occured
	std::set<int> subsetSpaces = getUnion(
		(cur.rowCol == Set::ROW) ? getRow(cur.affectingSubset1) : getCol(cur.affectingSubset1),
		(cur.rowCol == Set::ROW) ? getRow(cur.affectingSubset2) : getCol(cur.affectingSubset2)
	);

	//the boxes that caused the interaction
	std::set<int> affectingBoxSpaces = getUnion(
		getBox(cur.affectingBox1),
		getBox(cur.affectingBox2)
	);

	//the box that had candidates removed because of the interaction
	std::set<int> affectedBoxSpaces = getBox(cur.affectedBox);

	std::set<int> affectingSpaces = getIntersection(affectingBoxSpaces, subsetSpaces);
	std::set<int> affectedSpaces = getIntersection(affectedBoxSpaces, subsetSpaces);

	printf("STEP:block to block interaction\nThe value %d was able to be removed as a candidate from %s %d and %d within box %d. This is because, within box %d and box %d, the only places that %d could go were in %s %d and %d\n",
		cur.val,
		setToString(cur.rowCol).c_str(),
		cur.affectingSubset1 + 1,
		cur.affectingSubset2 + 1,
		cur.affectedBox + 1,
		cur.affectingBox1 + 1,
		cur.affectingBox2 + 1,
		cur.val,
		setToString(cur.rowCol).c_str(),
		cur.affectingSubset1 + 1,
		cur.affectingSubset2 + 1);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (affectedSpaces.contains(i * 9 + j)) {
				setColor(RED);
			}
			else if (affectingSpaces.contains(i * 9 + j)) {
				setColor(YELLOW);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
}

void Step::printNakedSubset(int puzzle[9][9]) 
{
	NakedSubset cur = this->nakedSubset;
	std::string vals;
	std::string affectingSpaces;
	std::string affectedSpaces;

	std::set<int>::iterator it;
	//add vals to a string (to be printed later)
	vals += "[";
	for (it = cur.vals.begin(); it != cur.vals.end(); it++) {
		vals += (char)(*it + 48);
		vals += ", ";
		//adds 'val, ' on the end of the string
	}
	vals = vals.substr(0, vals.size() - 2);//remove the extra ', '
	vals += "]";

	//add affecting spaces to a string (to be printed later)
	affectingSpaces += "[";
	for (it = cur.affectingSpaces.begin(); it != cur.affectingSpaces.end(); it++) {
		int row = *it / 9;
		int col = *it % 9;

		affectingSpaces += "(";
		affectingSpaces += (char)(row + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectingSpaces += ", ";
		affectingSpaces += (char)(col + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectingSpaces += ")";
		affectingSpaces += ", ";
		//adds '(row, col), ' on the end of the string
	}
	affectingSpaces = affectingSpaces.substr(0, affectingSpaces.size() - 2); //remove the extra ', '
	affectingSpaces += "]";

	//add affected spaces to a string (to be printed later)
	affectedSpaces += "[";
	for (it = cur.affectedSpaces.begin(); it != cur.affectedSpaces.end(); it++) {
		int row = *it / 9;
		int col = *it % 9;

		affectedSpaces += "(";
		affectedSpaces += (char)(row + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectedSpaces += ", ";
		affectedSpaces += (char)(col + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectedSpaces += ")";
		affectedSpaces += ", ";
		//addes '(row, col), ' on the end of the string
	}
	affectedSpaces = affectedSpaces.substr(0, affectedSpaces.size() - 2);
	affectedSpaces += "]";

	printf("STEP:naked subset\nthe candidates %s were able to be removed from %s,\nbecause within %s %d, exactly %d possible candidates existed within exactly %d spaces.\nThat combination forms a subset of candidates that can only exist within those spaces.\nthe spaces contining those candidates %s are %s,\nwhich means the remaining spaces in %s %d: %s cannot contain those values.\n",
		vals.c_str(),
		affectedSpaces.c_str(),
		setToString(cur.rowColBox).c_str(),
		cur.rowColBoxNum + 1,
		cur.vals.size(),
		cur.affectingSpaces.size(),
		vals.c_str(),
		affectingSpaces.c_str(),
		setToString(cur.rowColBox).c_str(),
		cur.rowColBoxNum + 1,
		affectedSpaces.c_str());
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (cur.affectingSpaces.contains(i * 9 + j)) {
				setColor(YELLOW);
			}
			else if (cur.affectedSpaces.contains(i * 9 + j)) {
				setColor(RED);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
}

void Step::printHiddenSubset(int puzzle[9][9]) 
{
	HiddenSubset cur = this->hiddenSubset;
	std::string subsetVals;
	std::string affectedSpaces;

	std::set<int>::iterator it;
	//add vals to a string (to be printed later)
	subsetVals += "[";
	for (it = cur.subsetVals.begin(); it != cur.subsetVals.end(); it++) {
		subsetVals += (char)(*it + 48);
		subsetVals += ", ";
		//adds 'val, ' on the end of the string
	}
	subsetVals = subsetVals.substr(0, subsetVals.size() - 2);//remove the extra ', '
	subsetVals += "]";

	//add affected spaces to a string (to be printed later)
	affectedSpaces += "[";
	for (it = cur.affectedSpaces.begin(); it != cur.affectedSpaces.end(); it++) {
		int row = *it / 9;
		int col = *it % 9;

		affectedSpaces += "(";
		affectedSpaces += (char)(row + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectedSpaces += ", ";
		affectedSpaces += (char)(col + 49); //adding 1 for display (so there is no row/col/box 0, then 48 for ascii)
		affectedSpaces += ")";
		affectedSpaces += ", ";
		//adds '(row, col), ' on the end of the string
	}
	affectedSpaces = affectedSpaces.substr(0, affectedSpaces.size() - 2);
	affectedSpaces += "]";

	printf("STEP:hidden subset\nAll the candidates except %s were able to be removed from the spaces %s \nbecause a 'hidden subset' was formed within %s %d containing the candidates %s.\nThis is because there were exactly %d candidates:%s which occured in only %d spaces:%s in %s %d. \nTherefore, those spaces may only be one of those candidates.\n",
		subsetVals.c_str(),
		affectedSpaces.c_str(),
		setToString(cur.rowColBox).c_str(),
		cur.rowColBoxNum + 1,
		subsetVals.c_str(),
		cur.subsetVals.size(),
		subsetVals.c_str(),
		cur.affectedSpaces.size(),
		affectedSpaces.c_str(),
		setToString(cur.rowColBox).c_str(),
		cur.rowColBoxNum + 1);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (cur.affectedSpaces.contains(i * 9 + j)) {
				setColor(RED);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
}

void Step::printXWing(int puzzle[9][9]) 
{
	XWing cur = this->xWing;

	//the set that contains the candidates to be removed
	Set rowColWithCandidates = cur.rowCol;

	//the set that doesn't contian the candidates to be removed
	Set rowColWithoutCandidates = (cur.rowCol == Set::ROW) ? Set::COL : Set::ROW;

	//the first subset number of one of the subsets (row or column) that contains the candidates
	int subset1 = (cur.rowCol == Set::ROW) ? cur.row1 : cur.col1;

	//the second subset number of one of the subsets (row or column) that contains the candidates
	int subset2 = (cur.rowCol == Set::ROW) ? cur.row2 : cur.col2;

	std::set<int> rowSpaces = getUnion(getRow(cur.row1), getRow(cur.row2));
	std::set<int> colSpaces = getUnion(getCol(cur.col1), getCol(cur.col2));

	std::set<int> affectingSpaces = (cur.rowCol == Set::ROW) ? colSpaces : rowSpaces;
	std::set<int> intersections = getIntersection(rowSpaces, colSpaces);

	printf("STEP:x-wing\nconsider the rows %d and %d, and the columns %d and %d.\nWithin those %ss the only spaces that contain the candidate %d are also within those %ss.\nTherefore, the occurences of %d in the final solution must be at\nthe intersection of the rows: %d, %d and the columns:%d, %d.\nbecause of this, we may remove %d as a candidate from all spaces in the %ss %d and %d that aren't at those intersections.\n",
		cur.row1 + 1,
		cur.row2 + 1,
		cur.col1 + 1,
		cur.col2 + 1,
		setToString(rowColWithoutCandidates).c_str(),
		cur.val,
		setToString(rowColWithCandidates).c_str(),
		cur.val,
		cur.row1 + 1,
		cur.row2 + 1,
		cur.col1 + 1,
		cur.col2 + 1,
		cur.val,
		setToString(rowColWithCandidates).c_str(),
		subset1 + 1,
		subset2 + 1);
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (intersections.contains(i * 9 + j)) {
				setColor(GREEN);
			} 
			else if (cur.affectedSpaces.contains(i * 9 + j)) {
				setColor(RED);
			} 
			else if (affectingSpaces.contains(i * 9 + j)) {
				setColor(YELLOW);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
}

void Step::printYWing(int puzzle[9][9]) 
{
	YWing cur = this->yWing;

	int baseRow = cur.base / 9;
	int baseCol = cur.base % 9;
	int wing1Row = cur.wing1 / 9;
	int wing1Col = cur.wing1 % 9;
	int wing2Row = cur.wing2 / 9;
	int wing2Col = cur.wing2 % 9;

	printf("STEP:y-wing\nThe space at (row %d, column %d) is the base of a y-wing where the wings are (row %d, column %d) and (row %d, column %d).\nBecause of it, we were able to remove %d as a candidate from the intersection of the row/column/boxes that each wing is a part of.\n",
		baseRow + 1,
		baseCol + 1,
		wing1Row + 1,
		wing1Col + 1,
		wing2Row + 1,
		wing2Col + 1,
		cur.candidate);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int space = i * 9 + j;
			if (space == cur.base || space == cur.wing1 || space == cur.wing2) {
				setColor(GREEN);
			}
			else if (cur.affectedSpaces.contains(space)) {
				setColor(RED);
			}
			else {
				setColor(BLUE);
			}

			std::cout << puzzle[i][j] << " ";
		}
		std::cout << std::endl;
	}
	setColor(WHITE);
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
	case StepID::YWING:
		printYWing(puzzle);
		break;
	}
}