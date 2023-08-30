/*cell.cpp*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * Cell class
 * 
 * Defines a cell in a maze, with four different sides (North, South, East, West)
 * 
 * Is either on the path from the maze entrance to the maze exit, or isn't
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

#include "cell.h"

#include <iostream>

/**--------------------------------------------------------------------------------------
 * Constructor
 * 
 * Creates a regular cell with no exits in any cardinal direction
 * 
 * @param[in] r Row index of cell
 * @param[in] c Column index of cell
 * --------------------------------------------------------------------------------------
*/
Cell::Cell(int r, int c)
  : m_rowLoc(r), m_colLoc(c)
{}

/**--------------------------------------------------------------------------------------
 * updateMyExit()
 * 
 * Given a direction, creates an exit in said direction
 * 
 * @param[in] dir Cardinal direction in which to create an exit
 * --------------------------------------------------------------------------------------
*/
void Cell::updateMyExit(int dir)
{
  m_exits[dir] = VALID_EXIT_NO_MARKS;
  m_numExits++;
}

/**--------------------------------------------------------------------------------------
 * markCellExit()
 * 
 * Given a direction, marks the corresponding exit in that direction, if it exists
 *     A value of 0 in a direction means that exit exists and has not been marked before
 *     A value of 1 in a direction means that exit has been marked once
 *     A value of 2 in a direction means that exit has been marked twice
 * 
 * @param[in] dir Cardinal direction of exit to mark
 * --------------------------------------------------------------------------------------
*/
void Cell::markCellExit(int dir)
{
  if(m_exits[dir] > INVALID_EXIT) // Exit exists
  {
    m_exits[dir]++;
  }
  else
  {
    std::cerr << "ERROR: Cell (" << m_rowLoc << ", " << m_colLoc << "), failed to mark exit: " << dir << " because the exit does not exist" << std::endl;
  }
}

/**--------------------------------------------------------------------------------------
 * isItThisCell()
 * 
 * Given a pair of cell indices, checks if the current cell is the same as the given cell
 * 
 * @param[in] row Row index of the cell that is checked against
 * @param[in] col Column index of the cell that is checked against
 * @return true if the indices are for a different cell, false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isItThisCell(int otherRow, int otherCol) const
{
  return m_rowLoc == otherRow && m_colLoc == otherCol; 
}

/**--------------------------------------------------------------------------------------
 * isCellEntranceAndJunction()
 * 
 * NOTE: Only intended for use on the entrance cell
 * Knowing that the current cell is the entrance to the maze, returns true if it has at 
 * least two exits, false otherwise
 *     This is an edge case: if the entrance cell has at least two exits, then it will be 
 *     considered a junction
 *     All other cells do not follow this rule, and should be checked by isJunction() 
 *     instead
 * 
 * @return true if the entrance cell is a junction, false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isCellEntranceAndJunction() const
{
  return m_numExits >= VALID_EXIT_TWO_MARKS;
}

/**--------------------------------------------------------------------------------------
 * isCellJunction()
 * 
 * Check if the cell is a junction (>2 exits)
 * 
 * @return true if the cell is a junction (>2 exits), false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isCellJunction() const
{
  return m_numExits > VALID_EXIT_TWO_MARKS;
}

/**--------------------------------------------------------------------------------------
 * isCellDeadEnd()
 * 
 * Check if the cell is a dead end (only 1 exit)
 * 
 * @return true if the cell is a dead end (only 1 exit), false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isCellDeadEnd() const
{
  return m_numExits < VALID_EXIT_TWO_MARKS;
}

/**--------------------------------------------------------------------------------------
 * isOnlyThisDirMarked()
 * 
 * Checks if the current cell has only one exit marked, and that it is in the given 
 * direction
 * 
 * @return true if only the exit in the given direction is marked, false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isOnlyThisDirMarked(int dir) const
{
  for(int i = 0; i < NUM_CARDINAL_DIRECTIONS; i++) // Checking all 4 cardinal directions
  {
    if(m_exits[i] > VALID_EXIT_NO_MARKS && i != dir)
    {
      return false;
    }
  }

  return true;
}

/**--------------------------------------------------------------------------------------
 * isThisDirMarkedTwice()
 * 
 * Checks if the current cell has the exit in the given direction marked twice
 * 
 * @param[in] dir Direction of exit to be checked
 * @return true if the exit in the given direction is marked twice, false otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isThisDirMarkedTwice(int dir) const
{
  /**
   * When isThisDirMarkedTwice() is called, the exit/entrance taken to the current cell 
   * has not yet been marked the second time
   *     Therefore the conditional checks for if the marks on the current exit is 
   *     greater than or equal to 1 
  */
  return m_exits[dir] > VALID_EXIT_ONE_MARK; 
}

/**--------------------------------------------------------------------------------------
 * isCellJunctionAllDirFilled()
 * 
 * NOTE: Should only be called if the current cell is a junction
 * If every exit to the junction except one has two marks, then the junction itself needs 
 * to be deleted from the traversed path
 *     If every exit to the junction except one has two marks, return true, false otherwise
 * 
 * @return true if all exits to the current cell except one are marked twice, false 
 * otherwise
 * --------------------------------------------------------------------------------------
*/
bool Cell::isCellJunctionAllDirFilled() const
{
  int numTwoMarks = 0;

  for(int i = 0; i < NUM_CARDINAL_DIRECTIONS; i++)
  {
    if(m_exits[i] > VALID_EXIT_ONE_MARK)
    {
      numTwoMarks++;
    }
  }

  return numTwoMarks == m_numExits - 1;
}

/**--------------------------------------------------------------------------------------
 * getDirFewestMarks()
 * 
 * From all valid exits to the current cell, finds the exit with the least amount of marks
 *     Returns the direction with the fewest marks (0, 1, or 2), 
 *     excluding -1 (exit in that direction does not exist)
 * 
 * @return an int representing the cardinal direction of the exit with the least marks
 * --------------------------------------------------------------------------------------
*/
int Cell::getDirFewestMarks() const
{
  int numMarks = 100; // High number of marks impossible to reach
  int dir = 0; // North by default

  for(int i = 0; i < NUM_CARDINAL_DIRECTIONS; i++)
  {
    if(m_exits[i] > INVALID_EXIT && m_exits[i] < numMarks)
    {
      numMarks = m_exits[i];
      dir = i;
    }
  }

  return dir;
}

/**--------------------------------------------------------------------------------------
 * findDirOnlyOtherExit()
 * 
 * NOTE: Only intended for use on cells that are part of a "passageway" (exactly two 
 * exits to the cell)
 *     Dead ends do not count as part of a "passageway"
 * Given the direction of an exit to the current cell, returns the direction of the only 
 * other exit
 * 
 * @param[in] dir Int representing the cardinal direction of one of two exits from the 
 * current maze
 * @return an int representing the cardinal direction of the only other exit to the cell
 * --------------------------------------------------------------------------------------
*/
int Cell::findDirOnlyOtherExit(int dir) const
{
  int otherDir = INVALID_EXIT;
  int numOtherExits = 0;

  for(int i = 0; i < NUM_CARDINAL_DIRECTIONS; i++)
  {
    if(m_exits[i] > INVALID_EXIT && i != dir)
    {
      otherDir = i;
      numOtherExits++;
    }
  }

  if(numOtherExits > 1)
  {
    std::cerr << "ERROR: findOnlyOtherExit error: number of exits other than the one given: " << numOtherExits << std::endl;
  }
  
  return otherDir;
}

/**--------------------------------------------------------------------------------------
 * getStringMarks()
 * 
 * Prints the current marks at each of the cell's exits
 * --------------------------------------------------------------------------------------
*/
std::string Cell::getStringMarks() const
{
  return "Marks... North: " + std::to_string(m_exits[0]) + ", South: " \
                            + std::to_string(m_exits[1]) + ", East: " \
                            + std::to_string(m_exits[2]) + ", West: " \
                            + std::to_string(m_exits[3]);
}