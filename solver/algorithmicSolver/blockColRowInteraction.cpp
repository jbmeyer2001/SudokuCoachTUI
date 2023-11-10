#include "../../include/AlgorithmicSolver.h"

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
		if (boardMap->removeCandidates(onlyRowOneComm, affectedSpaces, true, this->stepVal))
		{
			//don't update val, because we don't know the val. removeCandidates updates stepVal.
			updateStepBlockColRow(BoxSubset::TOP, Set::ROW, box, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the middle row of the box, record the step and return true if there is
		affectedSpaces = getDifference(getRow(row + 1), getBox(box));
		if (boardMap->removeCandidates(onlyRowTwoComm, affectedSpaces, true, this->stepVal))
		{
			updateStepBlockColRow(BoxSubset::MIDDLE, Set::ROW, box, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the bottom row of the box, record the step and return true if there is
		affectedSpaces = getDifference(getRow(row + 2), getBox(box));
		if (boardMap->removeCandidates(onlyRowThreeComm, affectedSpaces, true, this->stepVal))
		{
			updateStepBlockColRow(BoxSubset::BOTTOM, Set::ROW, box, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the left column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col), getBox(box));
		if (boardMap->removeCandidates(onlyColOneComm, affectedSpaces, true, this->stepVal))
		{
			updateStepBlockColRow(BoxSubset::LEFT, Set::COL, box, affectedSpaces);
			return true;
		}

		//see if there is a block to col/row interaction on the middle column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col + 1), getBox(box));
		if (boardMap->removeCandidates(onlyColTwoComm, affectedSpaces, true, this->stepVal))
		{
			updateStepBlockColRow(BoxSubset::MIDDLE, Set::COL, box, affectedSpaces);
			return true;

		}

		//see if there is a block to col/row interaction on the right column of the box, record the step and return true if there is
		affectedSpaces = getDifference(getCol(col + 2), getBox(box));
		if (boardMap->removeCandidates(onlyColThreeComm, affectedSpaces, true, this->stepVal))
		{
			updateStepBlockColRow(BoxSubset::RIGHT, Set::ROW, box, affectedSpaces);
			return true;
		}
	}
	return false;
}