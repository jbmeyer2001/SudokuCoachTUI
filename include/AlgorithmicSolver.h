#pragma once

#include <set>
#include <vector>

#include "BoardMap.h"
#include "Utility.h"

class AlgorithmicSolver
{
private:
	int puzzle[9][9] = { -1 };
	BoardMap* boardMap;

	enum class Step
	{
		NOSTEP = -1,
		SOLECANDIDATE = 0,
		UNIQUECANDIDATE = 1,
		BLOCKROWCOL = 2,
		BLOCKBLOCK = 3,
		NAKEDSUBSET = 4,
		HIDDENSUBSET = 5,
		XWING = 6,
		SOLVED = 7,
		CANNOTSOLVE = 8,
	};

	enum class BoxSubset
	{
		NA = -1,
		TOP = 0,
		BOTTOM = 1,
		LEFT = 2,
		RIGHT = 3,
		MIDDLE = 4,
	};

	Step step = Step::NOSTEP;					//name of step taken (unique candidate, hidden subset, block-block interaction, etc.)
	int stepRowNum1 = -1;						//location of value added
	int stepRowNum2 = -1;						//location of value added
	int stepColNum1 = -1;						//location of value added
	int stepColNum2 = -1;						//location of value added
	int stepVal = -1;							//value added or removed
	int stepSubsetNum1 = -1;					//contains 0-8, an indication of row/col/box number
	int stepSubsetNum2 = -1;					//contains 0-8, an indication of row/col/box number
	int stepSubsetNum3 = -1;					//contains 0-8, an indication of row/col/box number
	Set stepSet = Set::NA;						//contains row/col/box info
	BoxSubset stepBoxSubset1 = BoxSubset::NA;	//contains top/mid/bot/left/mid/right info
	BoxSubset stepBoxSubset2 = BoxSubset::NA;	//contains top/mid/bot/left/mid/right info
	BoxSubset stepBoxSubset3 = BoxSubset::NA;	//contains top/mid/bot/left/mid/right info
	std::set<int> stepAffectedSpaces = {};		//squares from which candidates may be removed
	std::set<int> stepVals = {};				//values (candidates) to be removed from affected squares

	bool soleCandidate(void);
	bool uniqueCandidate(void);
	bool checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set s);
	bool blockColRowInteraction(void);
	bool blockBlockInteraction(void);
	bool nakedSubset(void);
	bool hiddenSubset(void);
	std::set<int> getNextPartition(std::vector<int> spaces, unsigned short& prev);
	void clearStepInfo(void);
	bool XWing(void);
	bool XWingHelper(std::set<int> rows, std::set<int> cols, std::set<int> intersections);

	/*
	updateStepSoleCandidate updates the following:
	this->step					- sole candidate
	this->stepVal				- the value (1-9) placed by the most recent call to sole candidate
	this->stepColNum1			- the value (0-8) of the column that the value was placed in
	this->stepRowNum1			- the value (0-8) of the row that the value was placed in
	*/
	void updateStepSoleCandidate(int row, int col, int val);

	/*
	UpdateStepUniqueCandidate updates the following:
	this->step					- unique candidate
	this->stepVal				- the value (1-9) placed by the most recent call to unique candidate
	this->stepColNum1			- the value (0-8) of the column that the value was placed in
	this->stepRowNum1			- the value (0-8) of the row that the value was placed in
	this->stepSubsetNum1		- the value of the row/col/box in which puzzle['row', 'col'] was the only space that could be 'value'
	this->stepSet				- a string indicating whether stepSubsetNum1 was a row, a column, or a box
	*/
	void updateStepUniqueCandidate(int row, int col, int val, int subset, Set set);

	/*
	UpdateStepBlockColRow updates the following:
	this->step					- block col row
	this->stepBoxSubset1		- the box in which the interaction occured
	this->stepSet				- whether the interaction occured in a row or column
	this->stepSubsetNum			- the value (0-8) of that row or column
	this->stepAffectedSpaces	- the an std::set of the spaces (0-81) that were affected
	this->stepVal				- not updated in this function (rather, updated in remove candidates) but will indicate the value
								  that was removed from the affected spaces.
	*/
	void updateStepBlockColRow(BoxSubset boxLocation, Set set, int subset, std::set<int> affectedSpaces);

	/*
	blah blah blah
	*/
	void updateStepBlockBlock(BoxSubset boxSubset1, BoxSubset boxSubset2, BoxSubset boxSubset3, Set set, int subset1, int subset2, int subset3, std::set<int> affectedSpaces);

	/*
	blah blah blah
	*/
	void updateStepSubset(std::set<int> affectedSpaces, std::set<int> vals, Set set, int subset, Step step);

	/*
	updateStepXWing updates the following:
	this->step					- Xwing
	this->stepVal				- the value of the candidate that was removed from the affected spaces
	this->stepAffectedSpaces	- the spaces that had val removed from their candidates
	this->stepRowNum1			- to indicate one of the rows used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
	this->stepRowNum2			- to indicate the other row used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
	this->stepColNum1			- to indicate on eof the cols used in the x wing (not updated in this function, but updated by the time xwing finishes execution)
	this->stepColNum2			- to indicate the other colused in the x wign (not updated in this function, but updated by the time xwing finishes execution)
	this->stepSet				- to indicate whether the columns or the rows had some of their candidates removed
	*/
	void updateStepXWing(int candidate, std::set<int> affectedSpaces, Set set);

	std::string getStepBoxSubset(int which);

public:
	AlgorithmicSolver(int puzzle[9][9]);
	void nextStep(void);
	void solve(void);

	std::string getStep();
	std::string getStepSet();
	std::string getStepBoxSubset1();
	std::string getStepBoxSubset2();
	std::string getStepBoxSubset3();
	
	int getStepRowNum1();
	int getStepRowNum2();
	int getStepColNum1();
	int getStepColNum2();
	int getStepVal();
	int getStepSubsetNum1();
	int getStepSubsetNum2();
	int getStepSubsetNum3();
	std::set<int> getStepAffectedSpaces();
	std::set<int> getStepVals();
};

