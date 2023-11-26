#include "../../include/StepInfo.h"

void StepInfo::clearStepInfo(void)
{
	this->step = Step::NOSTEP;
	this->stepRowNum1 = -1;
	this->stepRowNum2 = -1;
	this->stepColNum1 = -1;
	this->stepColNum2 = -1;
	this->stepVal = -1;
	this->stepSubsetNum1 = -1;
	this->stepSubsetNum2 = -1;
	this->stepSubsetNum3 = -1;
	this->stepSet = Set::NA;
	this->stepBoxSubset1 = BoxSubset::NA;
	this->stepBoxSubset2 = BoxSubset::NA;
	this->stepBoxSubset3 = BoxSubset::NA;
	this->stepAffectedSpaces.clear();
	this->stepVals.clear();
}

void StepInfo::updateStepSoleCandidate(int row, int col, int val)
{
	this->stepRowNum1 = row;
	this->stepColNum1 = col;
	this->stepVal = val;
	this->step = Step::SOLECANDIDATE;
}

void StepInfo::updateStepUniqueCandidate(int row, int col, int val, int subset, Set set)
{
	this->stepRowNum1 = row;
	this->stepColNum1 = col;
	this->stepVal = val;
	this->stepSubsetNum1 = subset;
	this->stepSet = set;
	this->step = Step::UNIQUECANDIDATE;
}

void StepInfo::updateStepBlockColRow(BoxSubset boxLocation, Set set, int subset, std::set<int> affectedSpaces)
{
	this->stepBoxSubset1 = boxLocation;
	this->stepSet = set;
	this->stepSubsetNum1 = subset;
	this->stepAffectedSpaces = affectedSpaces;
	this->step = Step::BLOCKROWCOL;
}

void StepInfo::updateStepBlockBlock(BoxSubset boxSubset1, BoxSubset boxSubset2, BoxSubset boxSubset3, Set set, int subset1, int subset2, int subset3, std::set<int> affectedSpaces)
{
	this->stepBoxSubset1 = boxSubset1;
	this->stepBoxSubset2 = boxSubset2;
	this->stepBoxSubset3 = boxSubset3;
	this->stepSet = set;
	this->stepSubsetNum1 = subset1;
	this->stepSubsetNum2 = subset2;
	this->stepSubsetNum3 = subset3;
	this->stepAffectedSpaces = affectedSpaces;
	this->step = Step::BLOCKBLOCK;
}

void StepInfo::updateStepSubset(std::set<int> affectedSpaces, std::set<int> vals, Set set, int subset, Step step)
{
	this->stepAffectedSpaces = affectedSpaces;
	this->stepVals = vals;
	this->stepSet = set;
	this->stepSubsetNum1 = subset;
	this->step = step;
}

void StepInfo::updateStepXWing(int candidate, std::set<int> affectedSpaces, Set set)
{
	this->step = Step::XWING;
	this->stepVal = candidate;
	this->stepAffectedSpaces = affectedSpaces;
	this->stepSet = set;
}

std::string StepInfo::getStep()
{
	switch (this->step)
	{
	case Step::NOSTEP:
		return "NOSTEP";
	case Step::SOLECANDIDATE:
		return "SOLECANDIDATE";
	case Step::UNIQUECANDIDATE:
		return "UNIQUECANDIDATE";
	case Step::BLOCKROWCOL:
		return "BLOCKROWCOL";
	case Step::BLOCKBLOCK:
		return "BLOCKBLOCK";
	case Step::NAKEDSUBSET:
		return "NAKEDSUBSET";
	case Step::HIDDENSUBSET:
		return "HIDDENSUBSET";
	case Step::XWING:
		return "XWING";
	case Step::SOLVED:
		return "SOLVED";
	case Step::CANNOTSOLVE:
		return "CANNOTSOLVE";
	}

	return "";
}

std::string StepInfo::getStepSet()
{
	switch (this->stepSet)
	{
	case Set::NA:
		return "NA";
	case Set::ROW:
		return "ROW";
	case Set::COL:
		return "COL";
	case Set::BOX:
		return "BOX";
	}

	return "";
}

std::string StepInfo::getStepBoxSubset1()
{
	return getStepBoxSubset(1);
}

std::string StepInfo::getStepBoxSubset2()
{
	return getStepBoxSubset(2);
}

std::string StepInfo::getStepBoxSubset3()
{
	return getStepBoxSubset(3);
}

std::string StepInfo::getStepBoxSubset(int which)
{
	BoxSubset boxsubset;
	switch (which)
	{
	case 1:
		boxsubset = this->stepBoxSubset1;
		break;
	case 2:
		boxsubset = this->stepBoxSubset2;
		break;
	case 3:
		boxsubset = this->stepBoxSubset3;
		break;
	default:
		//TODO: ERROR HANDLING
		break;
	}

	switch (boxsubset)
	{
	case BoxSubset::NA:
		return "NA";
	case BoxSubset::BOTTOM:
		return "BOTTOM";
	case BoxSubset::MIDDLE:
		return "MIDDLE";
	case BoxSubset::TOP:
		return "TOP";
	case BoxSubset::LEFT:
		return "LEFT";
	case BoxSubset::RIGHT:
		return "RIGHT";
	};

	return "";
}

int StepInfo::getStepRowNum1()
{
	return this->stepRowNum1;
}

int StepInfo::getStepRowNum2()
{
	return this->stepRowNum2;
}

int StepInfo::getStepColNum1()
{
	return this->stepColNum1;
}

int StepInfo::getStepColNum2()
{
	return this->stepColNum2;
}

int StepInfo::getStepVal()
{
	return this->stepVal;
}

int StepInfo::getStepSubsetNum1()
{
	return this->stepSubsetNum1;
}

int StepInfo::getStepSubsetNum2()
{
	return this->stepSubsetNum2;
}

int StepInfo::getStepSubsetNum3()
{
	return this->stepSubsetNum3;
}

std::set<int> StepInfo::getStepAffectedSpaces()
{
	return this->stepAffectedSpaces;
}

std::set<int> StepInfo::getStepVals()
{
	return this->stepVals;
}
}