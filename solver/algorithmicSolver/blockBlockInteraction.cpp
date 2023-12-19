#include <unordered_map>

#include "AlgorithmicSolver.h"

/*
The key of the block to block interaction
is having a single candidate show up on exactly
two rows (or columns) of two boxes (that share
those rows/columns).
*/

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
	for (int box = 0; box < 9; box++) {
		int startIndex = (box / 3) * 27 + (box % 3) * 3;
		int row = startIndex / 9;
		int col = startIndex % 9;

		block b;

		//start with row
		std::set<int> rowOneCandidates = boardMap.getCandidates(getIntersection(getRow(row), getBox(box)));
		std::set<int> rowTwoCandidates = boardMap.getCandidates(getIntersection(getRow(row + 1), getBox(box)));
		std::set<int> rowThreeCandidates = boardMap.getCandidates(getIntersection(getRow(row + 2), getBox(box)));

		//row commonalities are the candidates that show up in exactly two of the three rows within this box
		b.rowOneAndTwoComm = getDifference(getIntersection(rowOneCandidates, rowTwoCandidates), rowThreeCandidates);
		b.rowOneAndThreeComm = getDifference(getIntersection(rowOneCandidates, rowThreeCandidates), rowTwoCandidates);
		b.rowTwoAndThreeComm = getDifference(getIntersection(rowTwoCandidates, rowTwoCandidates), rowOneCandidates);

		//next on to the columns
		std::set<int> colOneCandidates = boardMap.getCandidates(getIntersection(getCol(col), getBox(box)));
		std::set<int> colTwoCandidates = boardMap.getCandidates(getIntersection(getCol(col + 1), getBox(box)));
		std::set<int> colThreeCandidates = boardMap.getCandidates(getIntersection(getCol(col + 2), getBox(box)));

		//generate column commonalities in the same way
		b.colOneAndTwoComm = getDifference(getIntersection(colOneCandidates, colTwoCandidates), colThreeCandidates);
		b.colOneAndThreeComm = getDifference(getIntersection(colOneCandidates, colThreeCandidates), colTwoCandidates);
		b.colTwoAndThreeComm = getDifference(getIntersection(colTwoCandidates, colTwoCandidates), colOneCandidates);

		blocks[box] = b;
	}

	//for each box, we check the boxes it shares rows with (to see if they share any commonalities)
	//as well as the boxes it shares columns with.
	for (int box = 0; box < 9; box++) {
		int startIndex = (box / 3) * 27 + (box % 3) * 3;
		int row = startIndex / 9;
		int col = startIndex % 9;

		int numRowBox1 = (box - (box % 3)) + ((box + 1) % 3);
		int numRowBox2 = (box - (box % 3)) + ((box + 2) % 3);
		int numColBox1 = (box + 3) % 9;
		int numColBox2 = (box + 6) % 9;
		block rowBox1 = blocks[numRowBox1];
		block rowBox2 = blocks[numRowBox2];
		block colBox1 = blocks[numColBox1];
		block colBox2 = blocks[numColBox2];

		//get commonalities of boxes this box shares a row with
		std::set<int> rowOneAndTwoRemove = getIntersection(rowBox1.rowOneAndTwoComm, rowBox2.rowOneAndTwoComm);
		std::set<int> rowOneAndThreeRemove = getIntersection(rowBox1.rowOneAndThreeComm, rowBox2.rowOneAndThreeComm);
		std::set<int> rowTwoAndThreeRemove = getIntersection(rowBox1.rowTwoAndThreeComm, rowBox2.rowTwoAndThreeComm);
		
		//get commonalities of boxes this box shares a column with
		std::set<int> colOneAndTwoRemove = getIntersection(colBox1.colOneAndTwoComm, colBox2.colOneAndTwoComm);
		std::set<int> colOneAndThreeRemove = getIntersection(colBox1.colOneAndThreeComm, colBox2.colOneAndThreeComm);
		std::set<int> colTwoAndThreeRemove = getIntersection(colBox1.colTwoAndThreeComm, colBox2.colTwoAndThreeComm);

		//if any of the above six sets are not empty, we may remove those candidates from the corrosponding rows
		//or columns within this box. Only signal true if candidates were actually removed.
		std::set<int> affectedSpaces;
		affectedSpaces = getDifference(getBox(box), getRow(row + 2));
		int val;

		if (rowOneAndTwoRemove.size() > 0 && boardMap.removeCandidates(rowOneAndTwoRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, row, row + 1, numRowBox1, numRowBox2, Set::ROW);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getRow(row + 1));
		if (rowOneAndThreeRemove.size() > 0 && boardMap.removeCandidates(rowOneAndThreeRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, row, row + 2, numRowBox1, numRowBox2, Set::ROW);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getRow(row));
		if (rowTwoAndThreeRemove.size() > 0 && boardMap.removeCandidates(rowTwoAndThreeRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, row + 1, row + 2, numRowBox1, numRowBox2, Set::ROW);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col + 2));
		if (colOneAndTwoRemove.size() > 0 && boardMap.removeCandidates(colOneAndTwoRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, col, col + 1, numColBox1, numColBox2, Set::COL);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col + 1));
		if (colOneAndThreeRemove.size() > 0 && boardMap.removeCandidates(colOneAndThreeRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, col, col + 2, numColBox1, numColBox2, Set::COL);
			return true;
		}

		affectedSpaces = getDifference(getBox(box), getCol(col));
		if (colTwoAndThreeRemove.size() > 0 && boardMap.removeCandidates(colTwoAndThreeRemove, affectedSpaces, val)) {
			step.updateBlockBlock(val, box, col + 1, col + 2, numColBox1, numColBox2, Set::COL);
			return true;
		}
	}

	return false;
}