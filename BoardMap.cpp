#include "BoardMap.h"

BoardMap::BoardMap(int puzzle[9][9])
{
	for (int i = 0; i < 81; i++)
	{
		int row = i / 9;
		int col = i % 9;
		int box = (i / 27) * 3 + (i % 9) / 3;
		int val = puzzle[row][col];

		if (val != -1)
		{
			rows[row].insert(val);
			cols[col].insert(val);
			boxes[box].insert(val);
		}
	}
}

int BoardMap::mustBe(int spaceNum)
{
	int row = spaceNum / 9;
	int col = spaceNum % 9;
	int box = (spaceNum / 27) * 3 + (spaceNum % 9) / 3;

	std::set<int> all = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::set<int> cantBe;

	//I would love to use set_union and set_difference here
	//but I get the error 'namespace "std" has no member "set_difference"'
	//whenever I try to use it... Oh well, this should work too

	cantBe.insert(rows[row].begin(), rows[row].end());
	cantBe.insert(cols[col].begin(), cols[col].end());
	cantBe.insert(boxes[box].begin(), boxes[box].end());
	
	all.erase(cantBe.begin(), cantBe.end());
	
	if (all.size() == 1)
		return *all.begin();

	return -1;
}