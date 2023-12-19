#pragma once

bool duplicates(int array[9]);
bool isValid(int puzzle[9][9]);
int nextRow(int row_num, int column_num);
int nextColumn(int row_num, int column_num);
void solveRecursive(int puzzle[9][9], int rowNum, int colNum, int& solutions);
bool checkSudoku(int puzzle[9][9]);
