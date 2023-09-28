#include <unordered_map>

#include "AlgorithmicSolver.h"

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
		std::set<int> rowSpaces = getRow(row);
		rowSpaces.erase(space);
		std::set<int> colSpaces = getCol(col);
		colSpaces.erase(space);
		std::set<int> boxSpaces = getBox(box);
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
		get the spaces in the box that aren't in the row with the call to getDifference(getBox(box), getRow(row)).
		For example is we are looking at the middle row of the top left box, the set returned would be {0, 1, 2, 18, 19, 20}.

		The call to getCandidates gets all the candidates from the spaces in the set mentioned above.

		finally, the call to get difference gets all the values that are in the commonality of that row, but not anywhere else in the box.

		The reason the code does this is because in order for the values in the commonalities to be useful, they must not occur elsewhere in the box,
		see 'Block and Column / Row Interaction' at https://www.kristanix.com/sudokuepic/sudoku-solving-techniques.php to see why
		*/
		std::set onlyRowOneComm = getDifference(rowOneCommonalities, getCandidates(getDifference(getBox(box), getRow(row))));
		std::set onlyRowTwoComm = getDifference(rowTwoCommonalities, getCandidates(getDifference(getBox(box), getRow(row + 1))));
		std::set onlyRowThreeComm = getDifference(rowThreeCommonalities, getCandidates(getDifference(getBox(box), getRow(row + 2))));

		//next do the columns
		std::set<int> colOneCommonalities = getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		//remove commonalities that aren't useful
		std::set onlyColOneComm = getDifference(colOneCommonalities, getCandidates(getDifference(getBox(box), getBox(col))));
		std::set onlyColTwoComm = getDifference(colTwoCommonalities, getCandidates(getDifference(getBox(box), getBox(col + 1))));
		std::set onlyColThreeComm = getDifference(colThreeCommonalities, getCandidates(getDifference(getBox(box), getBox(col + 2))));

		//remove candidates from affected spaces
		std::set<int> affectedSpaces;
		affectedSpaces = getDifference(getRow(row), getBox(box));
		flag = flag || removeCandidates(onlyRowOneComm, affectedSpaces, Interaction::BLOCKROWCOL);

		affectedSpaces = getDifference(getRow(row + 1), getBox(box));
		flag = flag || removeCandidates(onlyRowTwoComm, affectedSpaces, Interaction::BLOCKROWCOL);

		affectedSpaces = getDifference(getRow(row + 2), getBox(box));
		flag = flag || removeCandidates(onlyRowThreeComm, affectedSpaces, Interaction::BLOCKROWCOL);

		affectedSpaces = getDifference(getCol(col), getBox(box));
		flag = flag || removeCandidates(onlyColOneComm, affectedSpaces, Interaction::BLOCKROWCOL);

		affectedSpaces = getDifference(getCol(col + 1), getBox(box));
		flag = flag || removeCandidates(onlyColTwoComm, affectedSpaces, Interaction::BLOCKROWCOL);

		affectedSpaces = getDifference(getCol(col + 2), getBox(box));
		flag = flag || removeCandidates(onlyColThreeComm, affectedSpaces, Interaction::BLOCKROWCOL);
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

bool AlgorithmicSolver::blockBlockInteraction(void)
{
	bool flag = false;

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
		b.rowOneAndTwoComm = getDifference(getIntersection(rowOneCommonalities, rowTwoCommonalities), getCandidates(getRow(row + 2)));
		b.rowOneAndThreeComm = getDifference(getIntersection(rowOneCommonalities, rowThreeCommonalities), getCandidates(getRow(row + 1)));
		b.rowTwoAndThreeComm = getDifference(getIntersection(rowTwoCommonalities, rowTwoCommonalities), getCandidates(getRow(row)));

		//next on to the columns
		std::set<int> colOneCommonalities = getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		b.colOneAndTwoComm = getDifference(getIntersection(colOneCommonalities, colTwoCommonalities), getCandidates(getCol(col + 2)));
		b.colOneAndThreeComm = getDifference(getIntersection(colOneCommonalities, colThreeCommonalities), getCandidates(getCol(col + 1)));
		b.colTwoAndThreeComm = getDifference(getIntersection(colTwoCommonalities, colTwoCommonalities), getCandidates(getCol(col)));

		blocks[box] = b;
	}

	for (int box = 0; box < 9; box++)
	{
		int startIndex = (box / 3) * 27 + (box % 3) * 3;
		int row = startIndex / 9;
		int col = startIndex % 9;

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

		std::set<int> affectedSpaces;
		affectedSpaces = getDifference(getBox(box), getRow(row + 2));
		flag = flag || removeCandidates(rowOneAndTwoRemove, affectedSpaces, Interaction::BLOCKBLOCK);

		affectedSpaces = getDifference(getBox(box), getRow(row + 1));
		flag = flag || removeCandidates(rowOneAndThreeRemove, affectedSpaces, Interaction::BLOCKBLOCK);

		affectedSpaces = getDifference(getBox(box), getRow(row));
		flag = flag || removeCandidates(rowTwoAndThreeRemove, affectedSpaces, Interaction::BLOCKBLOCK);

		affectedSpaces = getDifference(getBox(box), getCol(col + 2));
		flag = flag || removeCandidates(colOneAndTwoRemove, affectedSpaces, Interaction::BLOCKBLOCK);

		affectedSpaces = getDifference(getBox(box), getCol(col + 1));
		flag = flag || removeCandidates(colOneAndThreeRemove, affectedSpaces, Interaction::BLOCKBLOCK);

		affectedSpaces = getDifference(getBox(box), getCol(col));
		flag = flag || removeCandidates(colTwoAndThreeRemove, affectedSpaces, Interaction::BLOCKBLOCK);
	}

	return flag;
}

std::set<int> AlgorithmicSolver::getUnfilledSpaces(std::set<int> spaces)
{
	std::set<int> retval;

	std::set<int>::iterator it;
	for (it = spaces.begin(); it != spaces.end(); it++)
	{
		int space = *it;

		if (boardMap->unfilled.contains(space))
		{
			retval.emplace(space);
		}
	}

	return retval;
}

bool AlgorithmicSolver::checkSubsets(void)
{
	//check rows for naked subset
	for (int i = 0; i < 9; i++)
	{
		if (nakedSubset(getRow(i), Set::ROW))
		{
			return true;
		}
	}

	//check columns for naked subset
	for (int i = 0; i < 9; i++)
	{
		if (nakedSubset(getCol(i), Set::COL))
		{
			return true;
		}
	}

	//check boxes for naked subset
	for (int i = 0; i < 9; i++)
	{
		if (nakedSubset(getBox(i), Set::BOX))
		{
			return true;
		}
	}

	//check rows for hidden subset
	for (int i = 0; i < 9; i++)
	{
		if (hiddenSubset(getRow(i), Set::ROW))
		{
			return true;
		}
	}

	//check columns for hidden subset
	for (int i = 0; i < 9; i++)
	{
		if (hiddenSubset(getCol(i), Set::COL))
		{
			return true;
		}
	}

	//check boxes for hidden subset
	for (int i = 0; i < 9; i++)
	{
		if (hiddenSubset(getBox(i), Set::BOX))
		{
			return true;
		}
	}

	return false;
}

bool AlgorithmicSolver::nakedSubset(std::set<int> spaces, Set s)
{
	std::set<int> unfilledSpaces = getUnfilledSpaces(spaces);
	std::set<int>::iterator it1;

	for (it1 = unfilledSpaces.begin(); it1 != unfilledSpaces.end(); it1++)
	{
		std::set<int> candidates1 = boardMap->spaceCandidates[*it1];
		std::set<int> equivSpaces;
		equivSpaces.insert(*it1);
	
		std::set<int>::reverse_iterator it2;
		for (it2 = unfilledSpaces.rend(); it2 != std::make_reverse_iterator(it1); it2++)
		{
			std::set<int> candidates2 = boardMap->spaceCandidates[*it2];
			
			if (candidates1 == candidates2)
			{
				equivSpaces.emplace(*it2);
			}
		}

		if (equivSpaces.size() == candidates1.size())
		{
			if (removeCandidates(candidates1, getDifference(unfilledSpaces, equivSpaces), Interaction::NAKEDSUBSET))
			{
				return true;
			}
		}
	}
}

bool AlgorithmicSolver::hiddenSubset(std::set<int> spaces, Set s)
{

}

bool AlgorithmicSolver::removeCandidates(std::set<int> candidates, std::set<int> affectedSpaces, Interaction interaction)
{
	bool flag = false;
	while (!candidates.empty())
	{
		int val = *candidates.begin();

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

		candidates.erase(val);
	}
	return flag;
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