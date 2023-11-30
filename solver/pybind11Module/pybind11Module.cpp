#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "AlgorithmicSolver.h"
#include "Step.h"

namespace py = pybind11;
typedef py::array_t<int, py::array::c_style | py::array::forcecast> ndarray_int;

struct py_solver {
	Step step;
	AlgorithmicSolver* algorithmicSolver;

	//create an instance of AlgorithmicSolver using an ndarray of ints.
	//also create a step, and pass it to algorithmic solver as a reference, 
	//for easy access of step information
	py_solver(ndarray_int array)
	{
		if (array.ndim() != 2) {
			throw std::runtime_error("input needs to be 2D numpy array");
		}
		if (array.shape()[0] != 9 || array.shape()[1] != 9) {
			throw std::runtime_error("columns and rows should be size 9");
		}
		
		int data[9][9];
		std::memcpy(data, array.data(), array.size() * sizeof(int));

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (data[i][j] < 0 || data[i][j] > 9)
				{
					throw std::runtime_error("the sudoku contains an invalid number (all numbers must bo 0-9)");
				}
			}
		}

		algorithmicSolver = new AlgorithmicSolver(data, step);
	}

	void nextStep(void)
	{
		algorithmicSolver->nextStep();
	}

	std::set<int> getUnfilled(void)
	{
		return algorithmicSolver->getUnfilled();
	}
};

PYBIND11_MODULE(sudoku, m)
{
	py::class_<SoleCandidate>(m, "sole_candidate")
		.def(py::init<>())
		.def_property_readonly("val", ([](SoleCandidate soleCandidate) {return soleCandidate.val; }))
		.def_property_readonly("row", ([](SoleCandidate soleCandidate) {return soleCandidate.row; }))
		.def_property_readonly("col", ([](SoleCandidate soleCandidate) {return soleCandidate.col; }));
	py::class_<UniqueCandidate>(m, "unique_candidate")
		.def(py::init<>())
		.def_property_readonly("val", ([](UniqueCandidate uniqueCandidate) {return uniqueCandidate.val; }))
		.def_property_readonly("row", ([](UniqueCandidate uniqueCandidate) {return uniqueCandidate.row; }))
		.def_property_readonly("col", ([](UniqueCandidate uniqueCandidate) {return uniqueCandidate.col; }))
		.def_property_readonly("row_col_box_num", ([](UniqueCandidate uniqueCandidate) {return uniqueCandidate.rowColBoxNum; }))
		.def_property_readonly("row_col_box", ([](UniqueCandidate uniqueCandidate) {return uniqueCandidate.rowColBox; }));
	py::class_<BlockRowCol>(m, "block_row_col")
		.def(py::init<>())
		.def_property_readonly("val", ([](BlockRowCol blockRowCol) {return blockRowCol.val; }))
		.def_property_readonly("box_num", ([](BlockRowCol blockRowCol) {return blockRowCol.boxNum; }))
		.def_property_readonly("row_col_num", ([](BlockRowCol blockRowCol) {return blockRowCol.rowColNum; }))
		.def_property_readonly("box_subset", ([](BlockRowCol blockRowCol) {return blockRowCol.boxSubset; }))
		.def_property_readonly("row_col", ([](BlockRowCol blockRowCol) {return blockRowCol.rowCol; }))
		.def_property_readonly("affected_spaces", ([](BlockRowCol blockRowCol) {return blockRowCol.affectedSpaces; }));
	py::class_<BlockBlock>(m, "block_block")
		.def(py::init<>())
		.def_property_readonly("val", ([](BlockBlock blockBlock) {return blockBlock.val; }))
		.def_property_readonly("box1", ([](BlockBlock blockBlock) {return blockBlock.box1; }))
		.def_property_readonly("box2", ([](BlockBlock blockBlock) {return blockBlock.box2; }))
		.def_property_readonly("box3", ([](BlockBlock blockBlock) {return blockBlock.box3; }))
		.def_property_readonly("row_col", ([](BlockBlock blockBlock) {return blockBlock.rowCol; }))
		.def_property_readonly("box_subset1", ([](BlockBlock blockBlock) {return blockBlock.boxSubset1; }))
		.def_property_readonly("box_subset2", ([](BlockBlock blockBlock) {return blockBlock.boxSubset2; }))
		.def_property_readonly("box_subset3", ([](BlockBlock blockBlock) {return blockBlock.boxSubset3; }))
		.def_property_readonly("affected_spaces", ([](BlockBlock blockBlock) {return blockBlock.affectedSpaces; }));
	py::class_<NakedSubset>(m, "naked_subset")
		.def(py::init<>())
		.def_property_readonly("row_col_box_num", ([](NakedSubset nakedSubset) { return nakedSubset.rowColBoxNum; }))
		.def_property_readonly("row_col_box", ([](NakedSubset nakedSubset) { return nakedSubset.rowColBox; }))
		.def_property_readonly("vals", ([](NakedSubset nakedSubset) { return nakedSubset.vals; }))
		.def_property_readonly("affected_spaces", ([](NakedSubset nakedSubset) { return nakedSubset.affectedSpaces; }));
	py::class_<HiddenSubset>(m, "hidden_subset")
		.def(py::init<>())
		.def_property_readonly("row_col_box_num", ([](HiddenSubset hiddenSubset) { return hiddenSubset.rowColBoxNum; }))
		.def_property_readonly("row_col_box", ([](HiddenSubset hiddenSubset) { return hiddenSubset.rowColBox; }))
		.def_property_readonly("vals", ([](HiddenSubset hiddenSubset) { return hiddenSubset.vals; }))
		.def_property_readonly("affected_spaces", ([](HiddenSubset hiddenSubset) { return hiddenSubset.affectedSpaces; }));
	py::class_<XWing>(m, "xwing")
		.def(py::init<>())
		.def_property_readonly("val", ([](XWing xWing) { return xWing.val; }))
		.def_property_readonly("row1", ([](XWing xWing) { return xWing.row1; }))
		.def_property_readonly("row2", ([](XWing xWing) { return xWing.row2; }))
		.def_property_readonly("col1", ([](XWing xWing) { return xWing.col1; }))
		.def_property_readonly("col2", ([](XWing xWing) { return xWing.col2; }))
		.def_property_readonly("row_col", ([](XWing xWing) { return xWing.rowCol; }))
		.def_property_readonly("affected_spaces", ([](XWing xWing) { return xWing.affectedSpaces; }));
	py::class_<Step>(m, "step")
		.def(py::init<>())
		.def_property_readonly("name", &Step::getStep)
		.def_property_readonly("sole_candidate", &Step::getSoleCandidate)
		.def_property_readonly("unique_candidate", &Step::getUniqueCandidate)
		.def_property_readonly("block_row_col", &Step::getBlockRowCol)
		.def_property_readonly("block_block", &Step::getBlockBlock)
		.def_property_readonly("naked_subset", &Step::getNakedSubset)
		.def_property_readonly("hidden_subset", &Step::getHiddenSubset)
		.def_property_readonly("xwing", &Step::getXWing);
	py::class_<py_solver>(m, "solver")
		.def(py::init<ndarray_int>())
		.def_property_readonly("get_step", ([](py_solver solver) {return solver.step; }))
		.def("calculate_next_step", &py_solver::nextStep);
}