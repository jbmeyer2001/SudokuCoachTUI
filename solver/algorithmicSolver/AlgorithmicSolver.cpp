#include <unordered_map>
#include <string>
#include <iostream>

#include "../../include/AlgorithmicSolver.h"

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
	step = new Step();
}

bool AlgorithmicSolver::soleCandidate(void)
{
	std::set<int>::iterator it;
	for (it = boardMap->unfilled.begin(); it != boardMap->unfilled.end(); it++)
	{
		int space = *it;
		std::set<int> candidates = boardMap->getCandidates(space);
		if (candidates.size() == 1)
		{
			int row = space / 9;
			int col = space % 9;
			int val = *candidates.begin();

			//place number into puzzle
			puzzle[space / 9][space % 9] = val;
			boardMap->insert(space, val);

			//record step/technique used to get this square
			step->updateSoleCandidate(row, col, val);

			//return true, meaning we've made changes to the puzzle and updated the step
			return true;
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

		//if one call returns true, the rest won't execute
		flag = flag || checkSpaceUniqueCandidate(space, boxSpaces, row, Set::BOX);
		flag = flag || checkSpaceUniqueCandidate(space, rowSpaces, col, Set::ROW);
		flag = flag || checkSpaceUniqueCandidate(space, colSpaces, box, Set::COL);

		if (flag)
			return true;
	}
	
	return false;
}

bool AlgorithmicSolver::checkSpaceUniqueCandidate(int space, std::set<int> spaces, int subset, Set set)
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

		//record step/technique used to get this square
		step->updateUniqueCandidate(space / 9, space % 9, val, subset, set);

		//return true, meaning we've made changes to the puzzle and updated the step
		return true;
	}

	return false;
}

bool AlgorithmicSolver::blockColRowInteraction(void)
{
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
		std::set<int> rowOneCommonalities = boardMap->getCommonalities(startIndex, startIndex + 1, startIndex + 2);
		std::set<int> rowTwoCommonalities = boardMap->getCommonalities(startIndex + 9, startIndex + 10, startIndex + 11);
		std::set<int> rowThreeCommonalities = boardMap->getCommonalities(startIndex + 18, startIndex + 19, startIndex + 20);

		/*
		get the spaces in the box that aren't in the row with the call to getDifference(getBox(box), getRow(row)).
		For example is we are looking at the middle row of the top left box, the set returned would be {0, 1, 2, 18, 19, 20}.

		The call to getCandidates gets all the candidates from the spaces in the set mentioned above.

		finally, the call to get difference gets all the values that are in the commonality of that row, but nowhere else in the box. This causes the interaction.

		The reason the code does this is because in order for the values in the commonalities to be useful, they must not occur elsewhere in the box,
		see 'Block and Column / Row Step' at https://www.kristanix.com/sudokuepic/sudoku-solving-techniques.php to see why
		*/
		std::set onlyRowOneComm = getDifference(rowOneCommonalities, boardMap->getCandidates(getDifference(getBox(box), getRow(row))));
		std::set onlyRowTwoComm = getDifference(rowTwoCommonalities, boardMap->getCandidates(getDifference(getBox(box), getRow(row + 1))));
		std::set onlyRowThreeComm = getDifference(rowThreeCommonalities, boardMap->getCandidates(getDifference(getBox(box), getRow(row + 2))));

		//next do the columns
		std::set<int> colOneCommonalities = boardMap->getCommonalities(startIndex, startIndex + 9, startIndex + 18);
		std::set<int> colTwoCommonalities = boardMap->getCommonalities(startIndex + 1, startIndex + 10, startIndex + 19);
		std::set<int> colThreeCommonalities = boardMap->getCommonalities(startIndex + 2, startIndex + 11, startIndex + 20);

		std::set onlyColOneComm = getDifference(colOneCommonalities, boardMap->getCandidates(getDifference(getBox(box), getCol(col))));
		std::set onlyColTwoComm = getDifference(colTwoCommonalities, boardMap->getCandidates(getDifference(getBox(box), getCol(col + 1))));
		std::set onlyColThreeComm = getDifference(colThreeCommonalities, boardMap->getCandidates(getDifference(getBox(box), getCol(col + 2))));

		//see if there is a block to col/row interaction on the top row of the box, record the step and return true if there is
		std::set<int> affectedSpaces;
		affectedSpaces = getDifference(getRow(row), getBox(box));
		int val;

		if (boardMap->removeCandidates(onlyRowOneComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, row, Set::ROW, BoxSubset::MIDDLE, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the middle row of the box, record the step and return true if there is
		affectedSpaces = getDifference(getRow(row + 1), getBox(box));
		if (boardMap->removeCandidates(onlyRowTwoComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, row + 1, Set::ROW, BoxSubset::MIDDLE, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the bottom row of the box, record the step and return true if there is
		affectedSpaces = getDifference(getRow(row + 2), getBox(box));
		if (boardMap->removeCandidates(onlyRowThreeComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, row + 2, Set::ROW, BoxSubset::BOTTOM, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the left column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col), getBox(box));
		if (boardMap->removeCandidates(onlyColOneComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, col, Set::COL, BoxSubset::RIGHT, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the middle column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col + 1), getBox(box));
		if (boardMap->removeCandidates(onlyColTwoComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, col + 1, Set::COL, BoxSubset::RIGHT, affectedSpaces);
			return true;

		}

		//see if there is a block to col/row interaction on the right column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col + 2), getBox(box));
		if (boardMap->removeCandidates(onlyColThreeComm, affectedSpaces, val))
		{
			step->updateBlockRowCol(val, box, col + 2, Set::COL, BoxSubset::RIGHT, affectedSpaces);
			return true;
		}
	}
	return false;
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
		std::set<int> rowOneCandidates = boardMap->getCandidates(getIntersection(getRow(row), getBox(box)));
		std::set<int> rowTwoCandidates = boardMap->getCandidates(getIntersection(getRow(row + 1), getBox(box)));
		std::set<int> rowThreeCandidates = boardMap->getCandidates(getIntersection(getRow(row + 2), getBox(box)));

		//TODO: explain what code does here
		b.rowOneAndTwoComm = getDifference(getIntersection(rowOneCandidates, rowTwoCandidates), rowThreeCandidates);
		b.rowOneAndThreeComm = getDifference(getIntersection(rowOneCandidates, rowThreeCandidates), rowTwoCandidates);
		b.rowTwoAndThreeComm = getDifference(getIntersection(rowTwoCandidates, rowTwoCandidates), rowOneCandidates);

		//next on to the columns
		std::set<int> colOneCandidates = boardMap->getCandidates(getIntersection(getCol(col), getBox(box)));
		std::set<int> colTwoCandidates = boardMap->getCandidates(getIntersection(getCol(col + 1), getBox(box)));
		std::set<int> colThreeCandidates = boardMap->getCandidates(getIntersection(getCol(col + 2), getBox(box)));

		b.colOneAndTwoComm = getDifference(getIntersection(colOneCandidates, colTwoCandidates), colThreeCandidates);
		b.colOneAndThreeComm = getDifference(getIntersection(colOneCandidates, colThreeCandidates), colTwoCandidates);
		b.colTwoAndThreeComm = getDifference(getIntersection(colTwoCandidates, colTwoCandidates), colOneCandidates);

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
		int val;

		if (boardMap->removeCandidates(rowOneAndTwoRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, row, row + 1, row + 2, Set::ROW, BoxSubset::TOP, BoxSubset::MIDDLE, BoxSubset::BOTTOM, affectedSpaces);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getRow(row + 1));
		if (boardMap->removeCandidates(rowOneAndThreeRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, row, row + 2, row + 1, Set::ROW, BoxSubset::TOP, BoxSubset::BOTTOM, BoxSubset::MIDDLE, affectedSpaces);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getRow(row));
		if (boardMap->removeCandidates(rowTwoAndThreeRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, row + 1, row + 2, row, Set::ROW, BoxSubset::MIDDLE, BoxSubset::BOTTOM, BoxSubset::TOP, affectedSpaces);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col + 2));
		if (boardMap->removeCandidates(colOneAndTwoRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, col, col + 1, col + 2, Set::COL, BoxSubset::LEFT, BoxSubset::MIDDLE, BoxSubset::RIGHT, affectedSpaces);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col + 1));
		if (boardMap->removeCandidates(colOneAndThreeRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, col, col + 2, col + 1, Set::COL, BoxSubset::LEFT, BoxSubset::RIGHT, BoxSubset::MIDDLE, affectedSpaces);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col));
		if (boardMap->removeCandidates(colTwoAndThreeRemove, affectedSpaces, val))
		{
			step->updateBlockBlock(val, col + 1, col + 2, col, Set::COL, BoxSubset::MIDDLE, BoxSubset::RIGHT, BoxSubset::LEFT, affectedSpaces);
			return true;
		}
	}

	return false;
}

bool AlgorithmicSolver::nakedSubset(void)
{
	Set set;
	std::set<int> spaces;

	for (int i = 0; i < 27; i++)
	{
		switch (i % 9)
		{
		case 0:
			spaces = boardMap->getUnfilledSpaces(getRow(i / 9));
			set = Set::ROW;
			break;
		case 1:
			spaces = boardMap->getUnfilledSpaces(getCol(i / 9));
			set = Set::COL;
			break;
		case 2:
			spaces = boardMap->getUnfilledSpaces(getBox(i / 9));
			set = Set::BOX;
			break;
		}

		if (spaces.size() < 4) //TODO: explain this
			continue;

		std::set<int>::iterator it1;

		for (it1 = spaces.begin(); it1 != spaces.end(); it1++)
		{
			std::set<int> candidates1 = boardMap->spaceCandidates[*it1];
			std::set<int> equivSpaces;
			equivSpaces.insert(*it1);

			std::set<int>::iterator it2 = it1;
			std::advance(it2, 1);
			for (it2; it2 != spaces.end(); it2++)
			{
				std::set<int> candidates2 = boardMap->spaceCandidates[*it2];

				if (candidates1 == candidates2)
				{
					equivSpaces.emplace(*it2);
				}
			}

			if (equivSpaces.size() == candidates1.size()) //TODO think about moving this to teh above for loop
			{
				std::set<int> affectedSpaces = getDifference(spaces, equivSpaces);
				if (boardMap->removeCandidates(candidates1, affectedSpaces))
				{
					step->updateNakedSubset(i / 9, set, candidates1, affectedSpaces);
					return true;
				}
			}
		}
	}

	return false;
}

bool AlgorithmicSolver::hiddenSubset(void)
{
	Set set;
	std::set<int> spaces;

	for (int i = 0; i < 27; i++)
	{
		switch (i % 9)
		{
		case 0:
			spaces = boardMap->getUnfilledSpaces(getRow(i / 9));
			set = Set::ROW;
			break;
		case 1:
			spaces = boardMap->getUnfilledSpaces(getCol(i / 9));
			set = Set::COL;
			break;
		case 2:
			spaces = boardMap->getUnfilledSpaces(getBox(i / 9));
			set = Set::BOX;
			break;
		}

		if (spaces.size() < 4)
			continue;

		unsigned short prev = (unsigned short)2;
		std::vector<int> spacesVec(spaces.begin(), spaces.end());

		std::set<int> partition = getNextPartition(spacesVec, prev);
		while (!partition.empty())
		{
			std::set<int> other = getDifference(spaces, partition);

			std::set<int> partitionCandidates = boardMap->getCandidates(partition);
			std::set<int> otherCandidates = boardMap->getCandidates(other);

			if (getDifference(partitionCandidates, otherCandidates).size() == partition.size())
			{
				int unused = 0;
				if (boardMap->removeCandidates(otherCandidates, partition))
				{
					step->updateHiddenSubset(i / 9, set, partition, otherCandidates);
					return true;
				}
			}

			partition = getNextPartition(spacesVec, prev);
		}
	}

	return false; 
}

std::set<int> AlgorithmicSolver::getNextPartition(std::vector<int> spaces, unsigned short& prev)
{
	std::set<int> retval;
	uint16_t curInt;
	unsigned short cur;

getPartition:
	curInt = (uint16_t)prev + 1;
	cur = (unsigned short)curInt;

	if (curInt >= pow(2, spaces.size()) - 1)
	{
		return retval;
	}

	for (uint16_t i = 0; i < spaces.size(); i++)
	{
		uint16_t mask = pow(2, i);
		
		if ((prev & mask) == mask)
		{
			retval.insert(spaces[i]);
		}
	}

	if (retval.size() < 2 || retval.size() > (spaces.size() - 2))
	{
		retval.clear();
		prev = cur;
		goto getPartition;
	}

	prev = cur;
	return retval;
}

bool AlgorithmicSolver::XWing(void)
{
	//Step 1: select two rows and two columns, such that none of their (four) intersection points share a box
	for (int row1 = 0; row1 <= 5; row1++)
	{
		int row2Start = ((row1 / 3) * 3) + 3;
		for (int row2 = row2Start; row2 <= 8; row2++)
		{
			for (int col1 = 0; col1 <= 5; col1++)
			{
				int col2Start = ((col1 / 3) * 3) + 3;
				for (int col2 = col2Start; col2 <= 8; col2++)
				{
					/*
					There are now three sets that may be defined:
						- the set of the intersections of rows and columns (4 spaces)
						- the set of all spaces in both columns but NOT in the rows
						- the set of all spaces in both rows but NOT in the columns

					if there is a candidate that exists in ALL of the intersection spaces,
					but doesn't exist as a candidate in any of the spaces in one of the row or column 
					sets, then that candidate may be removed from all spaces in the other row/column set.
					*/

					//all spaces in selected rows, and selected columns
					std::set<int> rowSpaces = getUnion(getRow(row1), getRow(row2));
					std::set<int> colSpaces = getUnion(getCol(col1), getCol(col2));

					//intersections set
					std::set<int> intSpaces = getIntersection(rowSpaces, colSpaces);

					//remove 
					rowSpaces = getDifference(rowSpaces, intSpaces);
					colSpaces = getDifference(colSpaces, intSpaces);

					if (XWingHelper(rowSpaces, colSpaces, intSpaces, row1, row2, col1, col2))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool AlgorithmicSolver::XWingHelper(std::set<int> rowSpaces, std::set<int> colSpaces, std::set<int> intSpaces, int row1, int row2, int col1, int col2)
{
	//get the candidates from each of the above sets
	std::set<int> rowCandidates = boardMap->getCandidates(rowSpaces);
	std::set<int> colCandidates = boardMap->getCandidates(colSpaces);

	//convert intSpaces to vector so we can index
	std::vector<int> intSpacesVec(intSpaces.begin(), intSpaces.end());

	//get the candidates that exist in all of the intersection spaces
	std::set<int> intCandidates = getIntersection(boardMap->getCandidates(intSpacesVec[0]),
												  boardMap->getCandidates(intSpacesVec[1]),
												  boardMap->getCandidates(intSpacesVec[2]),
												  boardMap->getCandidates(intSpacesVec[3]));

	std::set<int>::iterator it;
	for (it = intCandidates.begin(); it != intCandidates.end(); it++)
	{
		int candidate = *it;
		bool inRows = rowCandidates.contains(candidate);
		bool inCols = colCandidates.contains(candidate);

		if (inRows ^ inCols)
		{
			std::set<int> affectedSpaces = inRows ? rowSpaces : colSpaces;

			//we can ignore return value, since we know removeCandidates will be successfull
			boardMap->removeCandidates(candidate, affectedSpaces); 
			step->updateXWing(candidate, row1, row2, col1, col2, inRows ? Set::ROW : Set::COL, affectedSpaces);
			return true;
		}
	}

	return false;
}

void AlgorithmicSolver::nextStep(void)
{
	step->clearStep();
	if (soleCandidate()) { return; }
	if (uniqueCandidate()) { return; }
	if (blockColRowInteraction()) { return; }
	if (blockBlockInteraction()) { return; }
	if (nakedSubset()) { return; }
	if (hiddenSubset()) { return; }
	if (XWing()) { return; }

	//update something to indicate we can't find another step
}

void AlgorithmicSolver::solve(void)
{
	int i = 0;
	
	//super jank, change later
	while (!isSolved(puzzle) && i < 200)
	{
		nextStep();
		std::cout << step->getStep() << std::endl; //for testing purposes
		i++;
	}

	if (isSolved(puzzle))
	{
		printPuzzle(puzzle);
	}
}