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

enum class StepID
{
	NA = -1,
	SOLVED = 0,
	UNSOLVEABLE = 1,
	CANTSOLVE = 2,
	SOLECANDIDATE = 3,
	UNIQUECANDIDATE = 4,
	BLOCKROWCOL = 5,
	BLOCKBLOCK = 6,
	NAKEDSUBSET = 7,
	HIDDENSUBSET = 8,
	XWING = 9,
	YWING = 10
};

//used to specify output color of console text
enum class Color
{
	GREEN = 10,
	BLUE = 11,
	RED = 12,
	MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15,
};

constexpr Color GREEN	= Color::GREEN;
constexpr Color BLUE	= Color::BLUE;
constexpr Color RED		= Color::RED;
constexpr Color MAGENTA = Color::MAGENTA;
constexpr Color YELLOW	= Color::YELLOW;
constexpr Color WHITE	= Color::WHITE;

bool isSolved(int puzzle[9][9]);
void copyPuzzle(int src[9][9], int dest[9][9]);
std::set<int> getRow(int row);
std::set<int> getCol(int col);
std::set<int> getBox(int box);
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2);
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2, std::set<int> set3, std::set<int> set4);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2, std::set<int> set3);
std::set<int> getDifference(std::set<int> set1, std::set<int> set2);
std::string setToString(Set set);
