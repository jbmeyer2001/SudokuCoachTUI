#pragma once

#include <unordered_map>
#include <set>

class BoardMap
{
private:
	std::unordered_map<int, std::set<int>> rows;
	std::unordered_map<int, std::set<int>> cols;
	std::unordered_map<int, std::set<int>> boxes;

public:
	BoardMap(int puzzle[9][9]);
	int mustBe(int spaceNum);
};

