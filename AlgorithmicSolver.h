#pragma once
class AlgorithmicSolver
{
private:
	int unsolvedPuzzle[9][9] = { -1 };
	int solvedPuzzle[9][9] = { -1 };

public:
	AlgorithmicSolver(int puzzle[9][9]);
};

