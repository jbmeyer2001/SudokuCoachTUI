#pragma once
#include<set>
void printPuzzle(int puzzle[9][9]);
bool isSolved(int puzzle[9][9]);
bool duplicates(int array[9]);
bool isValid(int puzzle[9][9]);
int nextRow(int row_num, int column_num);
int nextColumn(int row_num, int column_num);
void copyPuzzle(int fullPuzzle[9][9], int emptyPuzzle[9][9]);
std::set<int> sameRowColBox(int row, int col, int box);
std::set<int> getIntersection(std::set<int> set1, std::set<int> set2);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2);
std::set<int> getUnion(std::set<int> set1, std::set<int> set2, std::set<int> set3);
std::set<int> getDifference(std::set<int> set1, std::set<int> set2);