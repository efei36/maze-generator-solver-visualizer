/*cell.h*/

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

#pragma once

#include <array>
#include <string>
#include <assert.h>

/**--------------------------------------------------------------------------------------
 * Cell class
 * 
 * Defines a cell in the maze, with four sides (North, South, East, West)
 * Can be either:
 *     A regular cell
 *     An entrance or exit cell to the maze
 *     A path cell (on the path from the maze entrance to the maze exit)
 * --------------------------------------------------------------------------------------
*/
class Cell
{
public:
    /**--------------------------------------------------------------------------------------
     * Default Constructor
     * --------------------------------------------------------------------------------------
    */
    Cell(){};

    /**--------------------------------------------------------------------------------------
     * Constructor
     * 
     * Creates a regular cell with no exits in any cardinal direction
     * 
     * @param[in] r Row index of cell
     * @param[in] c Column index of cell
     * --------------------------------------------------------------------------------------
    */
    Cell(int r, int c);

    /**--------------------------------------------------------------------------------------
     * updateMyExit()
     * 
     * Given a direction, creates an exit in said direction
     * 
     * @param[in] dir Cardinal direction in which to create an exit
     * --------------------------------------------------------------------------------------
    */
    void updateMyExit(int dir);

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
    void markCellExit(int dir);

    /**--------------------------------------------------------------------------------------
     * isItThisCell()
     * 
     * Given a pair of cell indices, checks if the current cell is the same as the given cell
     * 
     * @param[in] row Row index of the cell that is checked against
     * @param[in] col Column index of the cell that is checked against
     * @return  true if the indices are for a different cell, false otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isItThisCell(int otherRow, int otherCol) const;
 
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
    bool isCellEntranceAndJunction() const;

    /**--------------------------------------------------------------------------------------
     * isCellJunction()
     * 
     * Check if the cell is a junction (>2 exits)
     * 
     * @return true if the cell is a junction (>2 exits), false otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isCellJunction() const;

    /**--------------------------------------------------------------------------------------
     * isCellDeadEnd()
     * 
     * Check if the cell is a dead end (only 1 exit)
     * 
     * @return true if the cell is a dead end (only 1 exit), false otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isCellDeadEnd() const;

    /**--------------------------------------------------------------------------------------
     * isOnlyThisDirMarked()
     * 
     * Checks if the current cell has only one exit marked, and that it is in the given 
     * direction
     * 
     * @return true if only the exit in the given direction is marked, false otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isOnlyThisDirMarked(int dir) const;

    /**--------------------------------------------------------------------------------------
     * isThisDirMarkedTwice()
     * 
     * Checks if the current cell has the exit in the given direction marked twice
     * 
     * @param[in] dir Direction of exit to be checked
     * @return true if the exit in the given direction is marked twice, false otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isThisDirMarkedTwice(int dir) const;

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
    bool isCellJunctionAllDirFilled() const;

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
    int getDirFewestMarks() const;

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
    int findDirOnlyOtherExit(int dir) const;

    /**--------------------------------------------------------------------------------------
     * labelAsPath()
     * 
     * Labels the current cell as part of the path from the maze entrance to the maze exit
     *     Changes to value of the boolean "amIOnPath" to true
     * --------------------------------------------------------------------------------------
    */
    void labelCellAsPath()
    {
        m_amIOnPath = true;
    }

    /**--------------------------------------------------------------------------------------
     * isCellOnPath()
     * 
     * Determines if the current cell is on the path from the maze entrance to the exit
     * 
     * @return true if the cell is on the path from the maze entrance to the maze exit, false 
     * otherwise
     * --------------------------------------------------------------------------------------
    */
    bool isCellOnPath() const
    {
        return m_amIOnPath;
    }

    /**--------------------------------------------------------------------------------------
     * getStringMarks()
     * 
     * Prints the current marks at each of the cell's exits
     * --------------------------------------------------------------------------------------
    */
    std::string getStringMarks() const;

private:
    int m_rowLoc;
    int m_colLoc;
    static const int NUM_CARDINAL_DIRECTIONS = 4;
    static const int INVALID_EXIT = -1;
    static const int VALID_EXIT_NO_MARKS = 0;
    static const int VALID_EXIT_ONE_MARK = 1;
    static const int VALID_EXIT_TWO_MARKS = 2;

    // If true, this cell is part of the path traversed from the entrance to the exit, false otherwise
    bool m_amIOnPath = false;

    /**
     * Array Indices 0: North, 1: South, 2: East, 3: West
     *     A value of -1 in a direction means an exit in that directions does not exist
     *     A value of 0 in a direction means that exit exists and has not been marked before
     *     A value of 1 in a direction means that exit has been marked once
     *     A value of 2 in a direction means that exit has been marked twice
    */
    std::array<int, NUM_CARDINAL_DIRECTIONS> m_exits{ INVALID_EXIT, INVALID_EXIT, INVALID_EXIT, INVALID_EXIT };
    int m_numExits = 0;
};