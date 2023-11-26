#include "BoardMap.h"
#include "Utility.h"

BoardMap::BoardMap(int puzzle[9][9])
{
	std::set<int> rows[9];
	std::set<int> cols[9];
	std::set<int> boxes[9];

	for (int i = 0; i < 81; i++)
	{
		int row = i / 9;
		int col = i % 9;
		int box = (i / 27) * 3 + (i % 9) / 3;
		int val = puzzle[row][col];

		if (val != -1 && val != 0)
		{
			rows[row].insert(val);
			cols[col].insert(val);
			boxes[box].insert(val);
		}
		else
		{
			unfilled.insert(i);
		}
	}

	std::set<int>::iterator it1;
	for (it1 = unfilled.begin(); it1 != unfilled.end(); it1++)
	{
		int spaceNum = *it1;
		int row = spaceNum / 9;
		int col = spaceNum % 9;
		int box = (spaceNum / 27) * 3 + (spaceNum % 9) / 3;

		std::set<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		std::set<int>::iterator it2;
		for (it2 = rows[row].begin(); it2 != rows[row].end(); it2++)
		{
			all.erase(*it2);
		}
		for (it2 = cols[col].begin(); it2 != cols[col].end(); it2++)
		{
			all.erase(*it2);
		}
		for (it2 = boxes[box].begin(); it2 != boxes[box].end(); it2++)
		{
			all.erase(*it2);
		}

		spaceCandidates[spaceNum] = all;
	}
}

void BoardMap::insert(int spaceNum, int val)
{
	int row = spaceNum / 9;
	int col = spaceNum % 9;
	int box = (spaceNum / 27) * 3 + (spaceNum % 9) / 3;

	unfilled.erase(spaceNum);
	spaceCandidates[spaceNum].clear();

	std::set<int> affectedSpaces = getUnion(getRow(row), getCol(col), getBox(box));

	std::set<int>::iterator it;
	for (it = affectedSpaces.begin(); it != affectedSpaces.end(); it++)
	{
		if (unfilled.find(*it) != unfilled.end())
		{
			spaceCandidates[*it].erase(val);
		}
	}
}

std::set<int> BoardMap::getCandidates(std::set<int> spaces)
{
	std::set<int> retval;
	std::set<int>::iterator it;

	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		std::set<int> candidates = getCandidates(*it);

		if (!candidates.empty())
		{
			retval.insert(candidates.begin(), candidates.end());
		}
	}

	return retval;
}

std::set<int> BoardMap::getCandidates(int space)
{
	std::set<int> retval;

	if (unfilled.contains(space))
	{
		retval.insert(spaceCandidates[space].begin(), spaceCandidates[space].end());
	}

	return retval;
}

//used if we want to only remove one of the candidates from all squares that contain it
bool BoardMap::removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces, int &value)
{
	bool flag = false;
	while (!candidates.empty())
	{
		int val = *candidates.begin();
		candidates.erase(val);
		flag = removeCandidates(val, affectedSpaces);

		if (flag)
		{
			value = val;
			break;
		}
	}

	return flag;
}

//used if we want to remove all possible candidates in the candidates set
bool BoardMap::removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces)
{
	bool flag = false;
	while (!candidates.empty())
	{
		int val = *candidates.begin();
		candidates.erase(val);
		flag = removeCandidates(val, affectedSpaces);
	}

	return flag;
}

bool BoardMap::removeCandidates(int candidate, std::set<int> affectedSpaces)
{
	bool flag = false;
	std::set<int>::iterator it;
	for (it = affectedSpaces.begin(); it != affectedSpaces.end(); it++)
	{
		//the space is unfilled, and one of the affected spaces contains a value that needs to be removed
		//because of the block to row/col interaction
		if (unfilled.contains(*it) && spaceCandidates[*it].contains(candidate))
		{
			spaceCandidates[*it].erase(candidate);
			flag = true;
		}
	}

	return flag;
}

std::set<int> BoardMap::getUnfilledSpaces(std::set<int> spaces)
{
	std::set<int> retval;

	std::set<int>::iterator it;
	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		int space = *it;

		if (unfilled.contains(space))
		{
			retval.emplace(space);
		}
	}

	return retval;
}

std::set<int> BoardMap::getCommonalities(int i1, int i2, int i3)
{
	std::set<int> commonalities;
	std::set<int> si1 = spaceCandidates[i1]; //TODO maybe come up with better variable names than si1, si2, si3
	std::set<int> si2 = spaceCandidates[i2];
	std::set<int> si3 = spaceCandidates[i3];

	//i1 & i2
	std::set<int> common1 = getIntersection(si1, si2);
	std::set<int> common2 = getIntersection(si1, si3);
	std::set<int> common3 = getIntersection(si2, si3);

	return getUnion(common1, common2, common3);
}