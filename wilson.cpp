/*wilson.cpp*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * Wilson's Algorithm
 * 
 * Given an empty (blank) maze, uses loop-erased random walks to fill out the maze
 * in an unbiased manner
 */

/**
 * MIT License
 * Copyright (c) 2023 Eric Fei
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "wilson.h"

#include <cstdlib>
#include <iostream>

/**--------------------------------------------------------------------------------------
 * clearInMaze()
 * 
 * Is given a 2D array representing which cells are "in" the maze
 *     true at a [row][col] index means the corresponding cell is "in" the maze
 *     true at a [row][col] index means the corresponding cell is "outside" the maze
 * "Clear" the record of cells in the maze by filling setting every value in the 2D array
 * to false
 * 
 * @param[in,out] inMaze 2D array of bools representing which cells are "in" the maze, is
 * updated to contain entirely false bools
 * --------------------------------------------------------------------------------------
*/
void clearInMaze(bool** inMaze, int rowLength, int colLength){
	for(int row = 0; row < rowLength; row++){
		for(int col = 0; col < colLength; col++){
			inMaze[row][col] = false;
		}
	}
}



/**--------------------------------------------------------------------------------------
 * clearOutsideMaze()
 * 
 * Is given a map of tuple<int, int> (representing cell indices) to themselves, where
 * the map represents the cells which are not "in" the maze
 *     In the map, the value will always be the same as the key. Therefore it is mapping 
 *     a set of cell indices unto themselves
 * "Clear" the record of cells outside the maze by filling the map with every cell from
 * the grid
 * 
 * @param[in,out] outOfMaze Map of tuple<int, int> (representing cell indices) to themselves
 * is updated to contain the indices of every cell in the grid
 * --------------------------------------------------------------------------------------
*/
void clearOutsideMaze(std::map<std::tuple<int, int>, std::tuple<int, int>>& outOfMaze, int numRows, int numCols){
	for(int row = 0; row < numRows; row++){
		for(int col = 0; col < numCols; col++){
			outOfMaze.emplace(std::make_tuple(row, col), std::make_tuple(row, col));
		}
	}
}



/**--------------------------------------------------------------------------------------
 * randomWalk()
 * 
 * Performs a random walk from a random cell not "in" the maze until it reaches a cell 
 * "in" the maze, recording the direction of travel for each cell traversed. Updates the 
 * existing direction of travel for a cell if it has already been traversed once and is 
 * being traversed again
 *     Modifies a given grid of ints, where the int represents the last direction traveled 
 *     at the cell with the associated grid indices
 * 
 * @param[in] inMaze		ROWCELLS x COLCELLS grid representing which cells are "in" 
 * the maze and which are not
 * @param[in] 		startRow		Row index of cell from which to start the random walk
 * @param[in] 		startCol		Column index of cell from which to start the random walk
 * @param[in,out] 	dirOfExit		Map of tuple<int, int> (representing cell indices) to 
 * ints (representing cardinal directions) indicating the last direction of exit from the 
 * cell, is updated to contain final travel path of the random walk
 * --------------------------------------------------------------------------------------
*/
void randomWalk(bool const* const* inMaze, int numRows, int numCols, int startRow, int startCol, std::map<std::tuple<int, int>, int>& dirOfExit)
{
	// Initiating the random walk from the given starting cell indices
	int curRow = startRow;
	int curCol = startCol;

	if(curRow < 0 || curRow >= numRows || curCol < 0 || curCol >= numCols)
	{
		std::cerr << "ERROR: randomWalk encountered cell indices outside of the maze grid: (Row: " << curRow << ", Col: " << curCol << ")" << std::endl;
	}
	else
	{
		// Repeat until reaching a cell "in" the maze
		while(inMaze[curRow][curCol] != true)
		{
			int randDir = Maze::INVALID_CARDINAL_DIRECTION;
			// Go in a random direction until reaching a valid cell
			while(true)
			{
				randDir  = std::rand() % 4;

				if(Maze::NORTH_DIRECTION == randDir && curRow != 0) 				// Moving North is valid
				{
					break;
				} 
				else if(Maze::SOUTH_DIRECTION == randDir && curRow != numRows - 1) // Moving South is valid
				{
					break;
				}
				else if(Maze::EAST_DIRECTION == randDir && curCol != numCols - 1) 	// Moving East is valid
				{
					break;
				}
				else if(Maze::WEST_DIRECTION == randDir && curCol != 0) 			// Moving West is valid
				{
					break;
				}
			}

			// Recording chosen direction as exit direction from current cell
			dirOfExit[std::make_tuple(curRow, curCol)] = randDir;

			switch(randDir)
			{
				case Maze::NORTH_DIRECTION: // Move North
					curRow -= 1;
					break;
				case Maze::SOUTH_DIRECTION:	// Move South
					curRow += 1;
					break;
				case Maze::EAST_DIRECTION:	// Move East
					curCol += 1;
					break;
				case Maze::WEST_DIRECTION:	// Move West
					curCol -= 1;
					break;
				default:
					std::cerr << "ERROR: randomWalk did not choose a random cardinal direction to move in: " << randDir <<std::endl;
			}

			if(curRow < 0 || curRow >= numRows || curCol < 0 || curCol >= numCols)
			{
				std::cerr << "ERROR: randomWalk encountered cell indices outside of the maze grid: (Row: " << curRow << ", Col: " << curCol << ")" << std::endl;
				break;
			}
		}
	}
}



/**--------------------------------------------------------------------------------------
 * createEntranceAndExit()
 * 
 * Given a "closed off" maze (no entrance or exit), marks its entrance and exit so that they are not too close to each other
 * 
 * @param[in] closedOffMaze A maze that has no entrance or exit cells
 * --------------------------------------------------------------------------------------
*/
void createEntranceAndExit(Maze& closedOffMaze)
{
	/**
	 * Marking two unique random cells on the edges to be the Entrance and Exit
	 *     IMPORTANT CAVEAT: the maze has at least 2 rows and at least 2 columns, otherwise the entrance and exit may be at the same cell
	*/
	if(closedOffMaze.getROWCELLS() >= 2 && closedOffMaze.getCOLCELLS() >= 2)
	{
		// Choosing entrance indices
		int entranceSide = std::rand() % 4;
		int entranceRow = Maze::INVALID_ROW_COL;
		int entranceCol = Maze::INVALID_ROW_COL;
		switch(entranceSide)
		{
			case Maze::NORTH_DIRECTION:	// Entrance is on North side
				entranceRow = 0;
				entranceCol = std::rand() % closedOffMaze.getCOLCELLS();
				break;
			case Maze::SOUTH_DIRECTION:	// Entrance is on South side
				entranceRow = closedOffMaze.getROWCELLS() - 1;
				entranceCol = std::rand() % closedOffMaze.getCOLCELLS();
				break;
			case Maze::EAST_DIRECTION:	// Entrance is on East side
				entranceRow = std::rand() % closedOffMaze.getROWCELLS();
				entranceCol = closedOffMaze.getCOLCELLS() - 1;
				break;
			case Maze::WEST_DIRECTION:	// Entrance is on West side
				entranceRow = std::rand() % closedOffMaze.getROWCELLS();
				entranceCol = 0;
				break;
			default:
				std::cerr << "ERROR: creatEntranceAndExit did not choose a valid entranceSide: " << entranceSide << std::endl;
		}

		// Choosing exit side
		int exitSide = std::rand() % 4;
		int exitRow = Maze::INVALID_ROW_COL;
		int exitCol = Maze::INVALID_ROW_COL;

		// First four checks are edge cases where the entrance is in a corner
		if(entranceRow == 0 && entranceCol == 0)
		{
			exitRow = closedOffMaze.getROWCELLS() - 1;
			exitCol = closedOffMaze.getCOLCELLS() - 1;
		}
		else if(entranceRow == 0 && entranceCol == closedOffMaze.getCOLCELLS() - 1)
		{
			exitRow = closedOffMaze.getROWCELLS() - 1;
			exitCol = 0;
		}
		else if(entranceRow == closedOffMaze.getROWCELLS() - 1 && entranceCol == 0)
		{
			exitRow = 0;
			exitCol = closedOffMaze.getCOLCELLS() - 1;
		}
		else if(entranceRow == closedOffMaze.getROWCELLS() - 1 && entranceCol == closedOffMaze.getCOLCELLS() - 1)
		{
			exitRow = 0;
			exitCol = 0;
		}
		else
		{
			while(exitSide == entranceSide) // Ensures Entrance and Exit will not land on the same side pt. 1
			{
				exitSide = std::rand() % 4;
			}

			if(Maze::NORTH_DIRECTION == exitSide)		// Exit is on North side
			{
				exitRow = 0;
				exitCol = std::rand() % closedOffMaze.getCOLCELLS();
				while(exitCol == entranceCol) // Ensures Entrance and Exit will not land on the same side pt. 2
				{
					exitCol = std::rand() % closedOffMaze.getCOLCELLS();
				}
			}
			else if(Maze::SOUTH_DIRECTION == exitSide)	// Exit is on South side
			{
				exitRow = closedOffMaze.getROWCELLS() - 1;
				exitCol = std::rand() % closedOffMaze.getCOLCELLS();
				while(exitCol == entranceCol) // Ensures Entrance and Exit will not land on the same side pt. 2
				{
					exitCol = std::rand() % closedOffMaze.getCOLCELLS();
				}
			}
			else if(Maze::EAST_DIRECTION == exitSide)	// Exit is on East side
			{
				exitRow = std::rand() % closedOffMaze.getROWCELLS();
				exitCol = closedOffMaze.getCOLCELLS() - 1;
				while(exitRow == entranceRow) // Ensures Entrance and Exit will not land on the same side pt. 2
				{
					exitRow = std::rand() % closedOffMaze.getROWCELLS();
				}
			}
			else										// Exit is on West side
			{
				exitRow = std::rand() % closedOffMaze.getROWCELLS();
				exitCol = 0;
				while(exitRow == entranceRow) // Ensures Entrance and Exit will not land on the same side pt. 2
				{
					exitRow = std::rand() % closedOffMaze.getROWCELLS();
				}
			}
		}
		
		// Marking entrance and exit
		closedOffMaze.labelMazeEntrance(entranceRow, entranceCol);
		closedOffMaze.labelMazeExit(exitRow, exitCol);
	}
	else
	{
		std::cout << "WARNING: Maze is too small to generate proper entrance and exit\n    Be aware that the generated entrance and exit may be at the same location" << std::endl;
		closedOffMaze.labelMazeEntrance(std::rand() % closedOffMaze.getROWCELLS(), std::rand() % closedOffMaze.getCOLCELLS());
		closedOffMaze.labelMazeExit(std::rand() % closedOffMaze.getROWCELLS(), std::rand() % closedOffMaze.getCOLCELLS());
	}
}

/**--------------------------------------------------------------------------------------
 * runWilson()
 * 
 * Given an "empty" maze with no passageways, entrances, or exits uses Wilson's Algorithm
 * to create an unbiased maze
 *     Repeatedly uses loop-erased random walks to "fill out" the maze, until every cell
 *     in the grid is connected to the maze
 * 
 * @param[in,out] blankMaze "empty" Maze object containing no passageways, entrances or 
 * exits, updates the Maze object so that every cell in the grid is connected to each 
 * other, and an entrance and exit cell both exist
 * --------------------------------------------------------------------------------------
*/
void runWilson(Maze& blankMaze)
{
	std::srand(time(NULL));

	int unvisitedCells = blankMaze.getROWCELLS() * blankMaze.getCOLCELLS();
	// false: cell is not in the maze, true: cell is in the maze
	bool** inMaze = new bool*[blankMaze.getROWCELLS()];
	for(int i = 0; i < blankMaze.getROWCELLS(); i++)
	{
		inMaze[i] = new bool[blankMaze.getCOLCELLS()];
	}

	std::map<std::tuple<int, int>, std::tuple<int, int>> notInMaze;

	clearInMaze(inMaze, blankMaze.getROWCELLS(), blankMaze.getCOLCELLS());
	clearOutsideMaze(notInMaze, blankMaze.getROWCELLS(), blankMaze.getCOLCELLS());

	// Setting random cell as "in" maze
	int randR = std::rand() % blankMaze.getROWCELLS();
	int randC = std::rand() % blankMaze.getCOLCELLS();

	if(randR < 0 || randR >= blankMaze.getROWCELLS() || randC < 0 || randC >= blankMaze.getCOLCELLS())
	{
		std::cerr << "ERROR: Wilson's Algorithm encountered cell indices outside of the maze grid: (Row: " << randR << ", Col: " << randC << ")" << std::endl;
	}
	else
	{
		inMaze[randR][randC] = true;
	    notInMaze.erase(std::make_tuple(randR, randC));
	    unvisitedCells -= 1;
	}

	/**
	 * Performing random walks until the entire maze is filled
	 *
	 * Start from a cell not "in" the maze, randomly walk until reaching a cell "in" the maze.
	 * Record the direction of travel for each cell traversed. 
	 * Updates the existing direction of travel for a cell if it has already been traversed once and is being traversed again.
	 * 
	 * Once done walking, start from the starting cell and travel along the recorded directions, adding each traversed
	 * cell to the maze and removing walls along the way. For each cell, decrement the number of unvisited cells by 1.
	*/
	while(unvisitedCells > 0 && notInMaze.empty() == false)
	{
		// Selecting a cell to initiate the random walk from
		std::tuple<int, int> startingCell = notInMaze.begin()->second;
		int curRow = std::get<0>(startingCell);
		int curCol = std::get<1>(startingCell);
	
		std::map<std::tuple<int, int>, int> walkPath;
		randomWalk(inMaze, blankMaze.getROWCELLS(), blankMaze.getCOLCELLS(), curRow, curCol, walkPath);

		if(curRow < 0 || curRow >= blankMaze.getROWCELLS() || curCol < 0 || curCol >= blankMaze.getCOLCELLS())
		{
			std::cerr << "ERROR: Wilson's Algorithm encountered cell indices outside of the maze grid before retracing the randomWalk: (Row: " << curRow << ", Col: " << curCol << ")" << std::endl;
			break;
		}
		else
		{
			/**
			 * Travel along the returned path
			 *     Add each traversed cell to the maze, and remove the appropriate wall.
			 *     For each wall removed, updated the corresponding entry in Cell->exits to indicate that an exit in that direction exists.
			*/
			while(inMaze[curRow][curCol] != true)
			{
				auto iter = walkPath.find(std::make_tuple(curRow, curCol));
				int curDir;
				if(iter != walkPath.end())
				{
					curDir = iter->second;
				}
				else
				{
					std::cerr << "ERROR: walkPath in Wilson's Algorithm was unable to find the cell with indices: (Row: " << curRow << ", Col: " << curCol << ")" << std::endl;
				}
				int nextCRow = curRow;
				int nextCCol = curCol;

				inMaze[curRow][curCol] = true; // Adding current cell to "in" maze
				notInMaze.erase(std::make_tuple(curRow, curCol)); // Removing current cell from "outside" maze

				// Finding indices of next cell in the traveled path
				int oppositeDir = Maze::INVALID_CARDINAL_DIRECTION;
				switch(curDir)
				{
					case Maze::NORTH_DIRECTION:	// Next cell is to the North
						nextCRow -= 1;
						oppositeDir = Maze::SOUTH_DIRECTION;
						break;
					case Maze::SOUTH_DIRECTION:	// Next cell is to the South
						nextCRow += 1;
						oppositeDir = Maze::NORTH_DIRECTION;
						break;
					case Maze::EAST_DIRECTION:	// Next cell is to the East
						nextCCol += 1;
						oppositeDir = Maze::WEST_DIRECTION;
						break;
					case Maze::WEST_DIRECTION:	// Next cell is to the West
						nextCCol -= 1;
						oppositeDir = Maze::EAST_DIRECTION;
						break;
					default:
						std::cerr << "ERROR: Wilson's Algorithm encountered a non-cardinal direction when retracing the randomWalk: " << curDir << std::endl;
						break;
				}

				blankMaze.updateCellExits(curRow, curCol, curDir);
				blankMaze.updateCellExits(nextCRow, nextCCol, oppositeDir);
				std::tuple<int, int, int, int> neighborsInOrder;

				// Formatting neighborsInOrder so that it matches to a key in the map of walls
				if(curRow < nextCRow)	// Current cell comes first in key
				{
					neighborsInOrder = std::make_tuple(curRow, curCol, nextCRow, nextCCol);
				}
				else if(curRow == nextCRow)
				{
					if(curCol < nextCCol)	// Current cell comes first in key
					{
						neighborsInOrder = std::make_tuple(curRow, curCol, nextCRow, nextCCol);
					}
					else					// Next cell comes first in key
					{
						neighborsInOrder = std::make_tuple(nextCRow, nextCCol, curRow, curCol);
					}
				}
				else					// Next cell comes first in key
				{
					neighborsInOrder = std::make_tuple(nextCRow, nextCCol, curRow, curCol);
				}
				blankMaze.connectNeighbors(neighborsInOrder);

				// Moving to next cell
				curRow = nextCRow;
				curCol = nextCCol;

				unvisitedCells -= 1;

				if(curRow < 0 || curRow >= blankMaze.getROWCELLS() || curCol < 0 || curCol >= blankMaze.getCOLCELLS())
				{
					std::cerr << "ERROR: Wilson's Algorithm encountered cell indices outside of the maze grid when retracing the randomWalk: (Row: " << curRow << ", Col: " << curCol << ")" << std::endl;
					break;
				}
			}
		}
	}
	std::cout << std::endl;

	// Maze not properly filled out error catcher
	if(notInMaze.empty() == false)
	{
		std::tuple<int, int> remainingCell = notInMaze.begin()->second;
		int row = std::get<0>(remainingCell);
		int col = std::get<1>(remainingCell);

		std::cerr << "ERROR: Wilson's Algorithm did not fill the maze out" \
				  << "\n      notInMaze remaining: " << notInMaze.size() \
				  << "\n      remaining cell indices: (" << row << ", " << col << ")" \
				  << "\n      inMaze status at remaining cell: " << inMaze[row][col] << "\n" << std::endl;
	}
	
	createEntranceAndExit(blankMaze);

	for(int i = 0; i < blankMaze.getROWCELLS(); i++)
	{
		delete inMaze[i];
	}

	delete[] inMaze;
}