#pragma once

#include<string>
#include<set>

enum class Set
{
	NA = -1,
	ROW = 0,
	COL = 1,
	BOX = 2,
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

void printPuzzle(int puzzle[9][9]);
bool isSolved(int puzzle[9][9]);
bool duplicates(int array[9]);
bool isValid(int puzzle[9][9]);
int nextRow(int row_num, int column_num);
int nextColumn(int row_num, int column_num);
void copyPuzzle(int fullPuzzle[9][9], int emptyPuzzle[9][9]);
std::set<int> getRow(int row);
std::set<int> getCol(int col);
std::set<int> getBox(int box);
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2);
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2, std::set<int> set3, std::set<int> set4);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2, std::set<int> set3);
std::set<int> getDifference(std::set<int> set1, std::set<int> set2);
std::string setToString(Set set);
std::string boxSubsetToString(BoxSubset boxSubset);
