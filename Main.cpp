#include "SudokuList.h"
#include "AlgorithmicSolver.h"
#include "Utility.h"

int main(void)
{
	//TODO: check for puzzle validity somewhere, if it's not already being done
	SudokuNode sudoku;
	sudoku.assignPuzzle("TestSudoku");

	int puzzle[9][9];
	sudoku.getUnsolvedPuzzle(puzzle);

	AlgorithmicSolver solver(puzzle);
	solver.solve();
}

/*
int main(void)
{
	std::string select;
	char select_char;
	std::string name;
	SudokuList list;

	//------------------------------------------------------------------------------------------------------------//
	//Start:
	//user inputs what action they would like to do, program uses switch statment and goto to branch to that location.
	//------------------------------------------------------------------------------------------------------------//
start:
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "          Add an unsolved sudoku:                                    (enter 1)" << std::endl;
	std::cout << "          Solve a sudoku currently in the system:                    (enter 2)" << std::endl;
	std::cout << "          View a solved sudoku:                                      (enter 3)" << std::endl;
	std::cout << "          Quit the application:                                      (enter 4)" << std::endl;
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	//-----------------------------------------------------------------------------------------------//
	//Program needs '\n' as a delimiter or else an invalid entry could send the program into an 
	//infinite loop because it won't pause for the user to input a new value before reading using cin.
	//
	//I know there are other ways to solve this issue.
	//But, the following logic allows me the ease of use of getline, with a switch statement.
	//-----------------------------------------------------------------------------------------------//
start_input:
	getline(std::cin, select, '\n');
	if (select.length() != 1)
	{
		std::cout << "          Invalid entry, try again:" << std::endl;
		goto start_input;
	}
	select_char = select[0]; //Because a switch statement can't use strings.

	switch (select_char)
	{
	case '1':
		goto add_sudoku;
	case '2':
		goto solve_sudoku;
	case '3':
		goto view_solved_sudoku;
	case '4':
		goto quit;
	default:
		std::cout << "          invalid entry, try again:" << std::endl;
		goto start_input;
	}




	//-------------------------------------------------------------------//
	//Add sudoku:
	//Runs the addSudoku method in the list object, then returns to start.
	//-------------------------------------------------------------------//
add_sudoku:
	list.addSudoku();
	goto start;




	//--------------------------------------------------------------------------------------------------------------//
	//Solve sudoku:
	//User selects which sudoku they would like to solve from a list displayed by the displayPuzzleNames method,
	//then the displayUnsolved method is called to show what the unsolved version is before branching to check_sudoku.
	//--------------------------------------------------------------------------------------------------------------//
solve_sudoku:
	std::cout << "          Type the name of the sudoku you would like to solve from the following list:" << std::endl;
	list.displayPuzzleNames();
	std::cout << std::endl;
	getline(std::cin, name, '\n');
	list.displayUnsolved(name);
	std::cout << "\n\n";
	goto check_sudoku;




	//-------------------------------------------------------------------------------------------//
	//View solved solution:
	//Displays all sudokus in the system, the user enters which one they would like to see solved, 
	//then program branches to display_solved_sudoku.
	//-------------------------------------------------------------------------------------------//
view_solved_sudoku:
	std::cout << "          Type the value of the sudoku you would like view solved from the following list:" << std::endl;
	list.displayPuzzleNames();
	std::cout << std::endl;
	getline(std::cin, name, '\n');
	goto display_solved_sudoku;




	//-----------------------------------------------------------------------------------//
	//Check sudoku:
	//Runs checkSudoku method and asks user what to do next, and branches to that location.
	//-----------------------------------------------------------------------------------//
check_sudoku:
	list.checkSudoku(name);
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "          Return to the start:                                   (enter 1)" << std::endl;
	std::cout << "          Quit the application:                                  (enter 2)" << std::endl;
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	//---------------------------------------------------------------------------------------------------//
		//Program needs '\n' as a delimiter or else an invalid entry could send the program into an 
		//infinite loop because it won't pause for the user to input a new value before reading using cin.
		//
		//I know there are other ways to solve this issue.
		//But, the following logic allows me the ease of use of getline, with a switch statement.
		//-----------------------------------------------------------------------------------------------//
check_sudoku_input:
	getline(std::cin, select, '\n');
	if (select.length() != 1)
	{
		std::cout << "          Invalid entry, try again:" << std::endl;
		goto check_sudoku_input;
	}
	select_char = select[0]; //Because a switch statement can't use strings.

	switch (select_char)
	{
	case '1':
		goto start;
	case '2':
		goto quit;
	default:
		std::cout << "          Invalid entry, try again:" << std::endl;
		goto check_sudoku_input;
	}




	//---------------------------------------------------------------------------------//
	//Display solved sudoku:
	//runs displaySolved method and asks user what to do next, branches to that location.
	//---------------------------------------------------------------------------------//
display_solved_sudoku:
	list.displaySolved(name);
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "          Return to the start?                                   (enter 1)" << std::endl;
	std::cout << "          Quit the application?                                  (enter 2)" << std::endl;
	std::cout << "          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

	//------------------------------------------------------------------------------------------------------------------//
		//Program needs '\n' as a delimiter or else an invalid entry could send the program into an 
		//infinite loop because it won't pause for the user to input a new value before reading using cin.
		//
		//I know there are ways to change the delimiter of cin.
		//But, the following logic allows me to get the ease of use of getline (because the delimiter is set in the function)
		//with a switch statement
		//------------------------------------------------------------------------------------------------------------------//
display_solved_sudoku_input:
	getline(std::cin, select, '\n');
	if (select.length() > 1)
	{
		std::cout << "          Invalid entry, try again:" << std::endl;
		goto display_solved_sudoku_input;
	}
	select_char = select[0];

	switch (select_char)
	{
	case '1':
		goto start;
	case '2':
		goto quit;
	default:
		std::cout << "          Invalid entry, try again:" << std::endl;
		goto start;
	}




quit:
	std::cout << "          Thanks for playing!" << std::endl;
	return 0;
}
*/