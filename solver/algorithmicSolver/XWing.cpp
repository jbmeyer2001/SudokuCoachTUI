#include "../../include/AlgorithmicSolver.h"

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

					if (XWingHelper(rowSpaces, colSpaces, intSpaces))
					{
						//update the step info we were unable to in helper function
						this->stepRowNum1 = row1;
						this->stepRowNum2 = row2;
						this->stepColNum1 = col1;
						this->stepColNum2 = col2;

						return true;
					}
				}
			}
		}
	}

	return false;
}

bool AlgorithmicSolver::XWingHelper(std::set<int> rowSpaces, std::set<int> colSpaces, std::set<int> intSpaces)
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
			updateStepXWing(candidate, affectedSpaces, inRows ? Set::ROW : Set::COL);
			return true;
		}
	}

	return false;
}