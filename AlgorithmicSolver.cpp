#include <unordered_map>

#include "AlgorithmicSolver.h"
#include "BoardMap.h"
#include "Utility.h"

AlgorithmicSolver::AlgorithmicSolver(int puzzle[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int val = puzzle[i][j];
			this->puzzle[i][j] = val;
		}
	}
	
	boardMap = new BoardMap(this->puzzle);
}

bool AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{

		int spaceNum = *it;
		std::set<int> candidates = boardMap->spaceCandidates[spaceNum];
		if (candidates.size() == 1)
		{
			int row = spaceNum / 9;
			int col = spaceNum % 9;
			int val = *candidates.begin();

			puzzle[row][col] = val;
			boardMap->insert(spaceNum, val);
			return true; //TODO: record this to keep track of steps taken
		}
	}

	return false;
}

bool AlgorithmicSolver::uniqueCandidate(void)
{
	bool flag = false;

	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{
		int space = *it;
		int row = space / 9;
		int col = space % 9;
		int box = (space / 27) * 3 + (space % 9) / 3;
		std::set<int> rowSpaces = sameRowColBox(row, -1, -1);
		rowSpaces.erase(space);
		std::set<int> colSpaces = sameRowColBox(-1, col, -1);
		colSpaces.erase(space);
		std::set<int> boxSpaces = sameRowColBox(-1, -1, box);
		boxSpaces.erase(space);

		flag = flag || checkSpaceUniqueCandidate(space, rowSpaces);
		flag = flag || checkSpaceUniqueCandidate(space, colSpaces);
		flag = flag || checkSpaceUniqueCandidate(space, boxSpaces);

		if (flag)
			return true;
	}
	
	return false;
}

bool AlgorithmicSolver::checkSpaceUniqueCandidate(int space, std::set<int> spaces) //TODO: add enum so we know if it is a row, column, or box
{
	std::set<int> candidates = boardMap->spaceCandidates[space];

	std::set<int>::iterator it;
	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		int val = *it;
		if (boardMap->unfilled.contains(val))
		{
			std::set<int> removeCandidates = boardMap->spaceCandidates[val];
			candidates = getDifference(candidates, removeCandidates);
		}	
	}

	if (candidates.size() == 1)
	{
		int val = *candidates.begin();
		puzzle[space / 9][space % 9] = val;
		boardMap->insert(space, val);
		return true; //TODO: record this to keep track of steps taken
	}

	return false;
}

bool AlgorithmicSolver::blockColRowInteraction(void)
{
	bool flag = false;
	for (int box = 0; box < 9; box++) //for each block
	{
		int startIndex = (box / 3) * 27 + (box % 3) * 3;
		int row = startIndex / 9;
		int col = startIndex % 9;
		/*
		get commonalities of each row/column within the box.

		a commonality is defined as a number that appears at least
		twice in any given row or column within the box.

		'commonality' is not a widespread term, but rather one 
		I have come up with to give a useful concept a name.
		*/
		
		std::set<int>::iterator it;

		//start with rows
		std::set<int> rowOneCommonalities = getCommonalities(startIndex, startIndex + 1, startIndex + 2);
		std::set<int> rowTwoCommonalities = getCommonalities(startIndex + 9, startIndex + 10, startIndex + 11);
		std::set<int> rowThreeCommonalities = getCommonalities(startIndex + 18, startIndex + 19, startIndex + 20);

		/*
		get the spaces in the box that aren't in the row with the call to getDifference(sameRowColBox(-1, -1, box), sameRowColBox(row, -1, -1)).
		For example is we are looking at the middle row of the top left box, the set returned would be {0, 1, 2, 18, 19, 20}.

		The call to getCandidates gets all the candidates from the spaces in the set mentioned above.

		finally, the call to get difference gets all the values that are in the commonality of that row, but not anywhere else in the box.

		The reason the code does this is because in order for the values in the commonalities to be useful, they must not occur elsewhere in the box,
		see 'Block and Column / Row Interaction' at https://www.kristanix.com/sudokuepic/sudoku-solving-techniques.php to see why
		*/
		std::set onlyRowOneComm = getDifference(rowOneCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(row, -1, -1))));
		std::set onlyRowTwoComm = getDifference(rowTwoCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(row + 1, -1, -1))));
		std::set onlyRowThreeComm = getDifference(rowThreeCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(row + 2, -1, -1))));

		//next do the columns
		std::set<int> colOneCommonalities = getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		//remove commonalities that aren't useful
		std::set onlyColOneComm = getDifference(colOneCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(-1, col, -1))));
		std::set onlyColTwoComm = getDifference(colTwoCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(-1, col + 1, -1))));
		std::set onlyColThreeComm = getDifference(colThreeCommonalities, getCandidates(getDifference(sameRowColBox(-1, -1, box), sameRowColBox(-1, -col + 2, -1))));

		flag = flag || removeCandidates(onlyRowOneComm, row, -1, box);
		flag = flag || removeCandidates(onlyRowTwoComm, row + 1, -1, box);
		flag = flag || removeCandidates(onlyRowThreeComm, row + 2, -1, box);
		flag = flag || removeCandidates(onlyColOneComm, -1, col, box);
		flag = flag || removeCandidates(onlyColTwoComm, -1, col + 1, box);
		flag = flag || removeCandidates(onlyColThreeComm, -1, col + 2, box);
	}
	return flag;
}

std::set<int> AlgorithmicSolver::getCommonalities(int i1, int i2, int i3)
{
	std::set<int> commonalities;
	std::set<int> si1 = boardMap->spaceCandidates[i1]; //TODO maybe come up with better variable names than si1, si2, si3
	std::set<int> si2 = boardMap->spaceCandidates[i2];
	std::set<int> si3 = boardMap->spaceCandidates[i3];
	
	//i1 & i2
	std::set<int> common1 = getIntersection(si1, si2);
	std::set<int> common2 = getIntersection(si1, si3);
	std::set<int> common3 = getIntersection(si2, si3);
	
	return getUnion(common1, common2, common3);
}

std::set<int> AlgorithmicSolver::getCandidates(std::set<int> spaces)
{
	std::set<int> retval;
	std::set<int>::iterator it;

	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		int val = *it;
		if (boardMap->unfilled.contains(val))
		{
			retval.insert(boardMap->spaceCandidates[val].begin(), boardMap->spaceCandidates[val].end());
		}
	}

	return retval;
}

bool AlgorithmicSolver::removeCandidates(std::set<int> commonalities, int row, int col, int box)
{
	int rowOrCol = (row > col) ? row : col;
	bool flag = false;
	while (!commonalities.empty())
	{
		int val = *commonalities.begin();
		std::set<int> affectedSpaces = getDifference(sameRowColBox(row, col, -1), sameRowColBox(-1, -1, box));

		std::set<int>::iterator it;
		for (it = affectedSpaces.begin(); it != affectedSpaces.end(); it++)
		{
			//the space is unfilled, and one of the affected spaces contains a value that needs to be removed
			//because of the block to row/col interaction
			if (boardMap->unfilled.contains(*it) && boardMap->spaceCandidates[*it].contains(val))
			{
				boardMap->spaceCandidates[*it].erase(val);
				//TODO: record this to keep track of steps taken
				flag = true;
			}
		}

		commonalities.erase(val);
	}
	return flag;
}

bool AlgorithmicSolver::blockBlockInteraction(void)
{
	typedef struct {
		std::set<int> rowOneAndTwoComm;
		std::set<int> rowOneAndThreeComm;
		std::set<int> rowTwoAndThreeComm;
		std::set<int> colOneAndTwoComm;
		std::set<int> colOneAndThreeComm;
		std::set<int> colTwoAndThreeComm;
	} block; 

	std::unordered_map<int, block> blocks;

	//create blocks map with all row and column commonalities
	for (int box = 0; box < 9; box++)
	{
		int startIndex = (box / 3) * 27 + (box % 3) * 3;
		int row = startIndex / 9;
		int col = startIndex % 9;

		block b;

		//start with row
		std::set<int> rowOneCommonalities = getCommonalities(startIndex, startIndex + 1, startIndex + 2);
		std::set<int> rowTwoCommonalities = getCommonalities(startIndex + 9, startIndex + 10, startIndex + 11);
		std::set<int> rowThreeCommonalities = getCommonalities(startIndex + 18, startIndex + 19, startIndex + 20);

		//TODO: explain what code does here
		b.rowOneAndTwoComm = getDifference(getIntersection(rowOneCommonalities, rowTwoCommonalities), getCandidates(sameRowColBox(row + 2, -1, -1)));
		b.rowOneAndThreeComm = getDifference(getIntersection(rowOneCommonalities, rowThreeCommonalities), getCandidates(sameRowColBox(row + 1, -1, -1)));
		b.rowTwoAndThreeComm = getDifference(getIntersection(rowTwoCommonalities, rowTwoCommonalities), getCandidates(sameRowColBox(row, -1, -1)));

		//next on to the columns
		std::set<int> colOneCommonalities = getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		b.colOneAndTwoComm = getDifference(getIntersection(colOneCommonalities, colTwoCommonalities), getCandidates(sameRowColBox(col + 2, -1, -1)));
		b.colOneAndThreeComm = getDifference(getIntersection(colOneCommonalities, colThreeCommonalities), getCandidates(sameRowColBox(col + 1, -1, -1)));
		b.colTwoAndThreeComm = getDifference(getIntersection(colTwoCommonalities, colTwoCommonalities), getCandidates(sameRowColBox(col, -1, -1)));

		blocks[box] = b;
	}

	for (int box = 0; box < 9; box++)
	{
		block rowBox1 = blocks[(box - (box % 3)) + ((box + 1) % 3)];
		block rowBox2 = blocks[(box - (box % 3)) + ((box + 2) % 3)];
		block colBox1 = blocks[(box + 3) % 9];
		block colBox2 = blocks[(box + 6) % 9];

		std::set<int> rowOneAndTwoRemove = getIntersection(rowBox1.rowOneAndTwoComm, rowBox2.rowOneAndTwoComm);
		std::set<int> rowOneAndThreeRemove = getIntersection(rowBox1.rowOneAndThreeComm, rowBox2.rowOneAndThreeComm);
		std::set<int> rowTwoAndThreeRemove = getIntersection(rowBox1.rowTwoAndThreeComm, rowBox2.rowTwoAndThreeComm);
		std::set<int> colOneAndTwoRemove = getIntersection(colBox1.colOneAndTwoComm, colBox2.colOneAndTwoComm);
		std::set<int> colOneAndThreeRemove = getIntersection(colBox1.colOneAndThreeComm, colBox2.colOneAndThreeComm);
		std::set<int> colTwoAndThreeRemove = getIntersection(colBox1.colTwoAndThreeComm, colBox2.colTwoAndThreeComm);


	}
}

void AlgorithmicSolver::solve(void)
{
	int i = 0;
	while (!isSolved(puzzle) && i < 80)
	{
		//
		if (soleCandidate()) { continue; }
		if (uniqueCandidate()) { continue; }
		if (blockColRowInteraction()) { continue; }
		i++;
	}

	if (isSolved(puzzle))
	{
		
	}
}