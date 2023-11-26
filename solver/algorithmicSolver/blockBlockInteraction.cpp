#include <unordered_map>

#include "AlgorithmicSolver.h"

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