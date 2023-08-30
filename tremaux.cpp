/*tremaux.cpp*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * Tremaux's Algorithm
 * 
 * Given a  maze, uses a DFS-esque approach to find a path from the entrance of
 * the maze to the exit
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

#include "tremaux.h"
#include "logger.h"

#include <iostream>

/**--------------------------------------------------------------------------------------
 * takeStep()
 * 
 * Take a "step" in the given direction
 *     Update the row and column values to "move" to the cell in the given direction
 * 
 * @param[in,out] row Row index of current cell, updated to row index of new cell after 
 * taking a step
 * @param[in,out] col Column index of current cell, updated to column index of new cell 
 * after taking a step
 * --------------------------------------------------------------------------------------
*/
void takeStep(int& row, int& col, int dir)
{
    switch(dir)
    {
        case Maze::NORTH_DIRECTION:
            row -= 1;
            break;
        case Maze::SOUTH_DIRECTION:
            row += 1;
            break;
        case Maze::EAST_DIRECTION:
            col += 1;
            break;
        case Maze::WEST_DIRECTION:
            col -= 1;
            break;
        default:
            std::cerr << "ERROR: Failed to take a step in a cardinal direction: " << dir << std::endl;
            break;
    }
}

/**--------------------------------------------------------------------------------------
 * oppositeDirection()
 * 
 * Takes a cardinal direction, returns its opposite
 * 
 * @param[in] dir Int representing a valid cardinal direction
 * @return int representing the opposite cardinal direction of dir
 * --------------------------------------------------------------------------------------
*/
int oppositeDirection(int dir)
{
    std::array<int, 4> oppositeDir = { Maze::SOUTH_DIRECTION, Maze::NORTH_DIRECTION, Maze::WEST_DIRECTION, Maze:: EAST_DIRECTION };

    return oppositeDir[dir];
}

/**--------------------------------------------------------------------------------------
 * backTrack()
 * 
 * Backtracks until reaching a junction or the beginning of the maze (which itself can 
 * be a junction)
 *     Takes the given traversed path, continuously removes the latest cell visited until 
 *     reaching a junction or the beginning of the maze
 * 
 * @param[in,out]   traversedPath       Stack of tuple<int, int, int> representing the 
 * current path travelled. Tuples are formatted as <row, col, direction of exit>. Updates 
 * the stack until reaching the previous junction
 * @param[in,out]   unolvedMaze         Maze object with passageways, an entrance and an 
 * exit, is updated with new marks on the previous junction cell
 * @param[in,out]   curRow              Current row index before backtracking, is updated 
 * to current row index after backtracking is finished
 * @param[in,out]   curCol              Current column index before backtracking, is 
 * updated to current column index after backtracking is finished
 * @param[in,out]   exitedPrevThrough   Int representing the cardinal direction used to 
 * exit the previous cell before backtracking starts, is updated to represent the cardinal 
 * direction used to exit the cell right before the previous junction cell when backtracking 
 * is finished
 * @param[in,out]   enteredCurThrough   Int representing the cardinal direction used to 
 * enter the current cell before backtracking starts, is updated to represent the cardinal 
 * direction used to enter latest junction cell after backtracking is finished
 * @param[in]       startRow            Row index of the maze entrance
 * @param[in]       startCol            Column index of the maze entrance
 * --------------------------------------------------------------------------------------
*/
void backTrack(std::stack<std::tuple<int, int, int>>& traversedPath, Maze& unsolvedMaze, \
               int& curRow, int& curCol, int& exitedPrevThrough, int& enteredCurThrough, int startRow, int startCol)
{
    // Start backtracking to return to the last junction (or to the maze entrance if it was the last junction)
    std::tuple<int, int, int> curBacktrackInstructions = traversedPath.top();
    while(true)
    {
        Cell& curBacktrackCell = unsolvedMaze.findCell(std::get<0>(curBacktrackInstructions), std::get<1>(curBacktrackInstructions));

        // When encountering a junction, exit the while loop
        if(curBacktrackCell.isCellJunction() || (curBacktrackCell.isItThisCell(startRow, startCol) && curBacktrackCell.isCellEntranceAndJunction()))
        {
            break;
        }

        traversedPath.pop();
        LOG_DEBUG("Backtrack: removed instruction (" << std::get<0>(curBacktrackInstructions) << ", " << std::get<1>(curBacktrackInstructions) \
                  << "), exitDir " << std::get<2>(curBacktrackInstructions) << " from the traversedPath stack")

        curBacktrackInstructions = traversedPath.top();
        curBacktrackCell = unsolvedMaze.findCell(std::get<0>(curBacktrackInstructions), std::get<1>(curBacktrackInstructions));
    }

    Cell& finalBacktrackCell = unsolvedMaze.findCell(std::get<0>(curBacktrackInstructions), std::get<1>(curBacktrackInstructions));

    // Mark the exit we last took from this junction
    exitedPrevThrough = std::get<2>(curBacktrackInstructions);
    finalBacktrackCell.markCellExit(exitedPrevThrough);

    // Update direction of entry into the current junction cell
    enteredCurThrough = exitedPrevThrough;
    exitedPrevThrough = oppositeDirection(enteredCurThrough);

    curRow = std::get<0>(curBacktrackInstructions);
    curCol = std::get<1>(curBacktrackInstructions);

    LOG_DEBUG("Backtrack: removed junction instruction (" << std::get<0>(curBacktrackInstructions) << ", " << std::get<1>(curBacktrackInstructions) \
              << "), exitDir " << std::get<2>(curBacktrackInstructions) << " from the traversedPath stack\n" \
              << "-------------------" << finalBacktrackCell.getStringMarks())
    // Pop the junction (or beginning of the maze) from the stack so we can traverse it again
    traversedPath.pop();

    if(traversedPath.empty())
    {
        LOG_DEBUG("-------------------Back to beginning")
    }
}

/**--------------------------------------------------------------------------------------
 * runTremaux()
 * 
 * Iteratively traverses maze, stopping at junctions and travelling down each path until 
 * either encountering the exit or reaching a dead end, upon which the algorithm will 
 * backtrack and try a different path
 *     Functions as a human-friendly DFS approach to maze navigation
 * 
 * @param[in,out] unsolvedMaze Maze object where the path from the entrance to the exit 
 * is unknown, updates the Maze object so that the cells on the path from the entrance to
 * the exit are labeled as path cells
 * --------------------------------------------------------------------------------------
*/
void runTremaux(Maze& unsolvedMaze)
{
    std::tuple<int, int> startCoords = unsolvedMaze.getEntrance();
    std::tuple<int, int> endCoords = unsolvedMaze.getExit();

    // Stack of traversed cells
    std::stack<std::tuple<int, int, int>> traversedPath;

    int startRow = std::get<0>(startCoords);
    int startCol = std::get<1>(startCoords);

    int endRow = std::get<0>(endCoords);
    int endCol = std::get<1>(endCoords);
    
    int curRow = startRow;
    int curCol = startCol;

    // Indicate which direction was used to enter the current cell
    int enteredCurThrough = Maze::INVALID_CARDINAL_DIRECTION;

    // Indicates which direction was used to exit the previous cell
    int exitedPrevThrough = Maze::INVALID_CARDINAL_DIRECTION;

    int numLoops = 0;
    while(true)
    {   
        Cell& curCell = unsolvedMaze.findCell(curRow, curCol);
        LOG_DEBUG("    Current cell: (" << curRow << ", " << curCol << "), entered from : " << enteredCurThrough << "\n" \
                  << "    --------Current cell " << curCell.getStringMarks())

        if(curRow == endRow && curCol == endCol)
        {
            traversedPath.push(std::make_tuple(curRow, curCol, 100));
            LOG_DEBUG("      Tremaux reached maze exit")
            break;
        }
        else if(curCell.isCellJunction() || (curCell.isItThisCell(startRow, startCol) && curCell.isCellEntranceAndJunction()))
        {
            if(enteredCurThrough == Maze::INVALID_CARDINAL_DIRECTION) // Just started traversing the maze, pick any valid direction
            {
                exitedPrevThrough = curCell.getDirFewestMarks();
                enteredCurThrough = oppositeDirection(exitedPrevThrough);

                curCell.markCellExit(exitedPrevThrough);

                traversedPath.push(std::make_tuple(curRow, curCol, exitedPrevThrough));
                LOG_DEBUG("    Added a instruction: (" << curRow << ", " << curCol << "), exitDir: " << exitedPrevThrough << " to the traversedPath stack")
                takeStep(curRow, curCol, exitedPrevThrough);
            }
            else if(curCell.isOnlyThisDirMarked(enteredCurThrough)) // Only the entrance to this cell we came through is marked, pick arbitrary exit direction
            {
                curCell.markCellExit(enteredCurThrough);

                exitedPrevThrough = curCell.getDirFewestMarks();
                enteredCurThrough = oppositeDirection(exitedPrevThrough);

                curCell.markCellExit(exitedPrevThrough);

                traversedPath.push(std::make_tuple(curRow, curCol, exitedPrevThrough));
                LOG_DEBUG("    Added a instruction: (" << curRow << ", " << curCol << "), exitDir: " << exitedPrevThrough << " to the traversedPath stack")
                takeStep(curRow, curCol, exitedPrevThrough);
            }
            else if(curCell.isThisDirMarkedTwice(enteredCurThrough)) // The entrance to this cell we came through is marked twice, pick exit direction with fewest marks
            {
                if(curCell.isCellJunctionAllDirFilled()) // Every exit to the junction except one is filled, remove the junction from the traversedPath and begin backtracking to previous junction
                {
                    exitedPrevThrough = curCell.getDirFewestMarks();
                    enteredCurThrough = oppositeDirection(exitedPrevThrough);

                    curCell.markCellExit(exitedPrevThrough);

                    // Dont push the current junction to traversedPath, instead start backtracking
                    LOG_DEBUG("From closed junction: started backtracking")
                    backTrack(traversedPath, unsolvedMaze, curRow, curCol, exitedPrevThrough, enteredCurThrough, startRow, startCol);
                }
                else
                {
                    exitedPrevThrough = curCell.getDirFewestMarks();
                    enteredCurThrough = oppositeDirection(exitedPrevThrough);

                    curCell.markCellExit(exitedPrevThrough);

                    traversedPath.push(std::make_tuple(curRow, curCol, exitedPrevThrough));
                    LOG_DEBUG("    Added a instruction: (" << curRow << ", " << curCol << "), exitDir: " << exitedPrevThrough << " to the traversedPath stack")
                    takeStep(curRow, curCol, exitedPrevThrough);
                }
            }
            else // The entrance to this cell we came through is marked once, and other exits have marks
            {
                // Backtrack to the last junction
                int temp = exitedPrevThrough;
                exitedPrevThrough = enteredCurThrough;
                enteredCurThrough = temp;

                LOG_DEBUG("Started backtracking")
                backTrack(traversedPath, unsolvedMaze, curRow, curCol, exitedPrevThrough, enteredCurThrough, startRow, startCol);
            }
        }
        else if(curCell.isItThisCell(startRow, startCol)) // Edge case where there is only one exit from the entrance cell
        {
            exitedPrevThrough = curCell.getDirFewestMarks();
            enteredCurThrough = oppositeDirection(exitedPrevThrough);

            traversedPath.push(std::make_tuple(curRow, curCol, exitedPrevThrough));
            LOG_DEBUG("    Added a instruction: (" << curRow << ", " << curCol << "), exitDir: " << exitedPrevThrough << " to the traversedPath stack")
            takeStep(curRow, curCol, exitedPrevThrough);
        }
        else if(curCell.isCellDeadEnd())
        {
            // Backtrack to the last junction
            int temp = exitedPrevThrough;
            exitedPrevThrough = enteredCurThrough;
            enteredCurThrough = temp;

            LOG_DEBUG("Started backtracking")
            backTrack(traversedPath, unsolvedMaze, curRow, curCol, exitedPrevThrough, enteredCurThrough, startRow, startCol);
        }
        else // Current cell is part of a passageway, continue down said passageway
        {
            exitedPrevThrough = curCell.findDirOnlyOtherExit(enteredCurThrough);
            enteredCurThrough = oppositeDirection(exitedPrevThrough);

            traversedPath.push(std::make_tuple(curRow, curCol, exitedPrevThrough));
            LOG_DEBUG("    Added a instruction: (" << curRow << ", " << curCol << "), exitDir: " << exitedPrevThrough << " to the traversedPath stack")
            takeStep(curRow, curCol, exitedPrevThrough);
        }
    }

    LOG_DEBUG("        BEFORE FINAL INSTRUCTIONS\n" \
              << "            Final instructions num remaining: " << traversedPath.size())
    // Return the set of final instructions
    while(!traversedPath.empty())
    {
        std::tuple<int, int, int> curInstruction = traversedPath.top();
        int pathRow = std::get<0>(curInstruction);
        int pathCol = std::get<1>(curInstruction);
        int pathDir = std::get<2>(curInstruction);
        
        Cell& pathCell = unsolvedMaze.findCell(pathRow, pathCol);
        pathCell.labelCellAsPath();
        
        traversedPath.pop();
        LOG_DEBUG("        Final instructions: traversedPath.pop(), num remaining: " << traversedPath.size())
    }
}
