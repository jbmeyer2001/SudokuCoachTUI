#pragma once

#include <set>

class BoardMap
{
private:
	std::set<int> rows[9];
	std::set<int> cols[9];
	std::set<int> boxes[9];

public:
	BoardMap(int puzzle[9][9]);
	int mustBe(int spaceNum);
	void insert(int spaceNum, int val);
};

