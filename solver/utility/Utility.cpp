#include <iostream>
#include <algorithm>
#include <iterator>

#include "Utility.h"

bool isSolved(int puzzle[9][9])
{
	//return true if no zeroes (empty spaces) exist in the puzzle
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (puzzle[i][j] == 0)
				return false;
		}
	}
	return true;
}

void copyPuzzle(int src[9][9], int dest[9][9])
{
	for(int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

//get all indexes in row row
std::set<int> getRow(int row)
{
	std::set<int> indexes;

	for (int i = 0; i < 9; i++) {
		indexes.insert(i + row * 9);
	}

	return indexes;
}

//get all indexes in column col
std::set<int> getCol(int col)
{
	std::set<int> indexes;

	for (int i = 0; i < 9; i++) {
		indexes.insert(i * 9 + col);
	}

	return indexes;
}

//get all indexes in box box
std::set<int> getBox(int box)
{
	std::set<int> indexes;

	for (int i = 0; i < 9; i++) {
		indexes.insert((i / 3) * 9 + (i % 3) + (box / 3) * 27 + (box % 3) * 3);
	}

	return indexes;
}

//wrapper function for std::set_intersection
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2)
{
	std::set<int> retval;
	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(retval, retval.end()));
	return retval;
}

//overloaded wrapper function for std::set_intersection with 3 input sets
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2, std::set<int> set3, std::set<int> set4)
{
	return getIntersection(getIntersection(set1, set2), getIntersection(set3, set4));
}

//wrapper function for std::set_union
std::set<int> getUnion(std::set<int> set1, std::set<int> set2)
{
	std::set<int> retval;
	std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(retval, retval.end()));
	return retval;
}

//overloaded wrapper function for std::set_union
std::set<int> getUnion(std::set<int> set1, std::set<int> set2, std::set<int> set3)
{
	std::set<int> retval, partUnion;
	std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(partUnion, partUnion.end()));
	std::set_union(partUnion.begin(), partUnion.end(), set3.begin(), set3.end(), std::inserter(retval, retval.end()));
	return retval;
}

//wrapper function forr std::set_difference
std::set<int> getDifference(std::set<int> set1, std::set<int> set2)
{
	std::set<int> retval;
	std::set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(retval, retval.end()));
	return retval;
}

//return string depending on input Set enum
std::string setToString(Set set)
{
	std::string ret;

	switch (set) {
	case Set::ROW:
		ret = "row";
		break;
	case Set::COL:
		ret = "column";
		break;
	case Set::BOX:
		ret = "box";
		break;
	default:
		ret = "NA";
		break;
	}

	return ret;
}
