#pragma once

bool checkSudoku(int puzzle[9][9]);
void solveRecursive(int puzzle[9][9], int rowNum, int colNum, int& solutions);

