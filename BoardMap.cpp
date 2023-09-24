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

	std::set<int> affectedSpaces = sameRowColBox(row, col, box);

	std::set<int>::iterator it;
	for (it = affectedSpaces.begin(); it != affectedSpaces.end(); it++)
	{
		if (unfilled.find(*it) != unfilled.end())
		{
			spaceCandidates[*it].erase(val);
		}
	}
}