/*maze.h*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * Maze class
 * 
 * Contains a 2d array of Cell objects, and a map of cell indices to Wall objects
 * 
 * Allows operation of Wilson's Algorithm to generate a random unbiased maze
 * Allows operation of Tremaux's Algorithm to find a path from the entrance to the exit of a maze
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

#include "cell.h"
#include "wall.h"

#include <map>
#include <tuple>

/**--------------------------------------------------------------------------------------
 * Maze class
 * 
 * Contains a 2d array of Cell objects, and a map of cell indices to Wall objects
 * Dimenstions are ROWCELLS x COLCELLS
 * --------------------------------------------------------------------------------------
*/
class Maze
{
public:
    /**--------------------------------------------------------------------------------------
     * Constructor
     * 
     * Creates an empty maze with user-inputed dimensions
     * 
     * @param[in] uRows User-specified number of rows in maze
     * @param[in] uCols User-specified number of columns in maze
     * --------------------------------------------------------------------------------------
    */
    Maze(int uRows, int uCols);

    /**--------------------------------------------------------------------------------------
     * Destructor
     * 
     * Destroys cellsGrid
     * --------------------------------------------------------------------------------------
    */
    ~Maze();

    /**--------------------------------------------------------------------------------------
     * print()
     * 
     * Prints the contents of the maze in text format to the terminal
     * --------------------------------------------------------------------------------------
    */
    void printMaze() const;

    /**--------------------------------------------------------------------------------------
     * updateCellExits()
     * 
     * Given a direction, updates the possible exits in a cell
     * 
     * @param[in] row Row index of cell to be modified
     * @param[in] col Column index of cell to be modified
     * @param[in] dir Cardinal direction in which to create an exit
     * --------------------------------------------------------------------------------------
    */
    void updateCellExits(int row, int col, int dir);

    /**--------------------------------------------------------------------------------------
     * connectNeighbors()
     * 
     * Opens the wall between the two cells
     * 
     * @param[in] neighborCells Tuple<int, int, int, int> representing cell indices for two 
     * cells first pair of ints are the row and col of the "first" cell second pair of ints 
     * are the row and col of the "second" cell order of cells is from left->right, top->bottom
     * --------------------------------------------------------------------------------------
    */
    void connectNeighbors(std::tuple<int, int, int, int> neighborCells);

    /**--------------------------------------------------------------------------------------
     * getEntrance()
     * 
     * Returns a tuple<int, int> representing the cell indices of the maze entrance
     * 
     * @return a tuple<int, int> representing location (row, col) of maze entrance
     * --------------------------------------------------------------------------------------
    */
    std::tuple<int, int> getEntrance() const;

    /**--------------------------------------------------------------------------------------
     * getExit()
     * 
     * Returns a tuple<int, int> representing the cell indices of the maze exit 
     * 
     * @return a tuple<int, int representing location (row, col) of maze exit
     * --------------------------------------------------------------------------------------
    */
    std::tuple<int, int> getExit() const;

    /**--------------------------------------------------------------------------------------
     * labelMazeEntrance()
     * 
     * Labels a cell as the entrance to the maze
     * 
     * @param[in] row Row index of cell to be labeled as entrance
     * @param[in] col Column index of cell to be labeled as entrance
     * --------------------------------------------------------------------------------------
    */
    void labelMazeEntrance(int row, int col);

    /**--------------------------------------------------------------------------------------
     * labelMazeExit()
     * 
     * Labels a cell as the exit of the maze
     * 
     * @param[in] row Row index of cell to be labeled as exit
     * @param[in] col Column index of cell to be labeled as exit
     * --------------------------------------------------------------------------------------
    */
    void labelMazeExit(int row, int col);

    /**--------------------------------------------------------------------------------------
     * findCell()
     * 
     * Given a row and column, returns a reference to the corresponding cell in the maze
     * 
     * @param[in] row Row index of cell
     * @param[in] col Column index of cell
     * @return a reference to a Cell object in the maze
     * --------------------------------------------------------------------------------------
    */
    Cell& findCell(int row, int col);

    /**--------------------------------------------------------------------------------------
     * getWallsMap()
     * 
     * Returns a reference to the map of walls in the maze
     * 
     * @return a reference to a Map of tuple<int, int, int, int> to Wall objects
     * --------------------------------------------------------------------------------------
    */
    std::map<std::tuple<int, int, int, int>, Wall>& getWallsMap();

    /**--------------------------------------------------------------------------------------
     * getROWCELLS()
     * 
     * Returns a constant reference to the number of rows in the maze
     * 
     * @return a constant reference to ROWCELLS
     * --------------------------------------------------------------------------------------
    */
    const int& getROWCELLS() const
    {
        return ROWCELLS;
    }

    /**--------------------------------------------------------------------------------------
     * getCOLCELLS()
     * 
     * Returns a constant reference to the number of columns in the maze
     * 
     * @return a constant reference to COLCELLS
     * --------------------------------------------------------------------------------------
    */
    const int& getCOLCELLS() const
    {
        return COLCELLS;
    }
    
    /**
     * Integers representing the four cardinal directions
     *     0: North, 1: South, 2: East, 3: West
    */
    static const int NORTH_DIRECTION = 0;
    static const int SOUTH_DIRECTION = 1;
    static const int EAST_DIRECTION = 2;
    static const int WEST_DIRECTION = 3;

    /**
     * Placeholder values denoting invalid or uninitialized variables
    */
    static const int INVALID_CARDINAL_DIRECTION = -1;
    static const int INVALID_ROW_COL = -1;

private:
    Cell** m_cellsGrid;
    
    // Number of rows and number of columns should each always be at least 2 in order to ensure a suitable maze
    const int ROWCELLS;
    const int COLCELLS;

    // Tuple: <cellA row index, cellA col index, cellB row index, cellB col index>
    std::map<std::tuple<int, int, int, int>, Wall> m_neighborsToWallMap;

    int m_entranceCoords[2] = {INVALID_ROW_COL, INVALID_ROW_COL};
    int m_exitCoords[2] = {INVALID_ROW_COL, INVALID_ROW_COL};
};