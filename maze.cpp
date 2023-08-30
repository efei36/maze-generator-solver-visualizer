/*maze.cpp*/

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

#include <iostream>
#include <string>

#include "maze.h"
#include "logger.h"

/**--------------------------------------------------------------------------------------
 * Constructor
 * 
 * Creates an empty maze with user-inputed dimensions
 * 
 * @param[in] uRows User-specified number of rows in maze
 * @param[in] uCols User-specified number of columns in maze
 * --------------------------------------------------------------------------------------
*/
Maze::Maze(int uRows, int uCols)
    : ROWCELLS(uRows), COLCELLS(uCols)
{
    m_cellsGrid = new Cell*[ROWCELLS];
    for(int i = 0; i < ROWCELLS; i++)
    {
        m_cellsGrid[i] = new Cell[COLCELLS];
    }

    // Populating cell grid
    for(int row = 0; row < ROWCELLS; row++){
        for(int col = 0; col < COLCELLS; col++){
            m_cellsGrid[row][col] = Cell(row, col);
        }
    }

    // Creating vertical walls
    for(int row = 0; row < ROWCELLS; row++){
        for(int col = 0; col < COLCELLS - 1; col++){
            m_neighborsToWallMap.emplace(std::make_tuple(row, col, row, col + 1), Wall(row, col, row, col + 1));
        }
    }

    // Creating horizontal walls
    for(int row = 0; row < ROWCELLS - 1; row++){
        for(int col = 0; col < COLCELLS; col++){
            m_neighborsToWallMap.emplace(std::make_tuple(row, col, row + 1, col), Wall(row, col, row + 1, col));
        }
    }
}

/**--------------------------------------------------------------------------------------
 * Destructor
 * 
 * Destroys cellsGrid
 * --------------------------------------------------------------------------------------
*/
Maze::~Maze()
{
    for(int i = 0; i < ROWCELLS; i++)
    {
        delete m_cellsGrid[i];
    }

    delete[] m_cellsGrid;
}

/**--------------------------------------------------------------------------------------
 * print()
 * 
 * Prints the contents of the maze in text format to the terminal
 * --------------------------------------------------------------------------------------
*/
void Maze::printMaze() const
{
    for(int row = 0; row < ROWCELLS; row++){
        std::string cellsAndVertWalls = "";
        std::string horizWalls = "";
        for(int col = 0; col < COLCELLS; col++){
            // Printing cells
            if(row == m_entranceCoords[0] && col == m_entranceCoords[1])
            {
                cellsAndVertWalls += "I ";
            }
            else if(row == m_exitCoords[0] && col == m_exitCoords[1])
            {
                cellsAndVertWalls += "O ";
            }
            else if(m_cellsGrid[row][col].isCellOnPath())
            {
                cellsAndVertWalls += "W ";
            }
            else
            {
                cellsAndVertWalls += "C ";
            }

            // Printing existing vertical walls
            auto vertWallIter = m_neighborsToWallMap.find(std::make_tuple(row, col, row, col + 1));
            if(vertWallIter != m_neighborsToWallMap.end()){
                if(vertWallIter->second.checkIsOpen()){ // Wall is open
                    cellsAndVertWalls += "  ";
                }
                else{                                 // Wall is closed
                    cellsAndVertWalls += "| ";
                }
            }

            // Finding existing horizontal walls
            auto horizWallIter = m_neighborsToWallMap.find(std::make_tuple(row, col, row + 1, col));
            if(horizWallIter != m_neighborsToWallMap.end()){
                if(horizWallIter->second.checkIsOpen()){ // Wall is open
                    horizWalls += "    ";
                }
                else{                                  // Wall is closed
                    horizWalls += "-   ";
                }
            }
        }

        LOG_DEBUG(cellsAndVertWalls)
        LOG_DEBUG(horizWalls)
    }

    if(m_entranceCoords[0] != INVALID_ROW_COL && m_entranceCoords[1] != INVALID_ROW_COL)
    {
        LOG_DEBUG("Entrance I: (" << m_entranceCoords[0] << ", " << m_entranceCoords[1] << ")")
    }
    if(m_exitCoords[0] != INVALID_ROW_COL && m_exitCoords[1] != INVALID_ROW_COL)
    {
        LOG_DEBUG("Exit O: (" << m_exitCoords[0] << ", " << m_exitCoords[1] << ")")
    }
}

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
void Maze::updateCellExits(int row, int col, int dir)
{
    if(row < 0 || row >= ROWCELLS || col < 0 || col >= COLCELLS)
    {
       std::cerr << "ERROR: updateCellExits() did not find the cell (" << row << ", " << col << ")" << std::endl;  
    }

    m_cellsGrid[row][col].updateMyExit(dir);
}

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
void Maze::connectNeighbors(std::tuple<int, int, int, int> neighborCells)
{
    auto iter = m_neighborsToWallMap.find(neighborCells);
    if(iter != m_neighborsToWallMap.end())
    {
        iter->second.openWall();
    }
    else
    {
       std::cerr << "ERROR: connectNeighbors() did not find a wall" << std::endl;   
    }
}

/**--------------------------------------------------------------------------------------
 * getEntrance()
 * 
 * Returns a tuple<int, int> representing the cell indices of the maze entrance
 * 
 * @return a tuple<int, int> representing location (row, col) of maze entrance
 * --------------------------------------------------------------------------------------
*/
std::tuple<int, int> Maze::getEntrance() const
{
    return std::make_tuple(m_entranceCoords[0], m_entranceCoords[1]);
}

/**--------------------------------------------------------------------------------------
 * getExit()
 * 
 * Returns a tuple<int, int> representing the cell indices of the maze exit 
 * 
 * @return a tuple<int, int representing location (row, col) of maze exit
 * --------------------------------------------------------------------------------------
*/
std::tuple<int, int> Maze::getExit() const
{
    return std::make_tuple(m_exitCoords[0], m_exitCoords[1]);
}

/**--------------------------------------------------------------------------------------
 * labelMazeEntrance()
 * 
 * Labels a cell as the entrance to the maze
 * 
 * @param[in] row Row index of cell to be labeled as entrance
 * @param[in] col Column index of cell to be labeled as entrance
 * --------------------------------------------------------------------------------------
*/
void Maze::labelMazeEntrance(int row, int col)
{
    if(row < 0 || row >= ROWCELLS || col < 0 || col >= COLCELLS)
    {
       std::cerr << "ERROR: labelMazeEntrance() did not find the cell (" << row << ", " << col << ")" << std::endl;  
    }

    m_entranceCoords[0] = row;
    m_entranceCoords[1] = col;
}

/**--------------------------------------------------------------------------------------
 * labelMazeExit()
 * 
 * Labels a cell as the exit of the maze
 * 
 * @param[in] row Row index of cell to be labeled as exit
 * @param[in] col Column index of cell to be labeled as exit
 * --------------------------------------------------------------------------------------
*/
void Maze::labelMazeExit(int row, int col)
{
    if(row < 0 || row >= ROWCELLS || col < 0 || col >= COLCELLS)
    {
       std::cerr << "ERROR: labelMazeExit() did not find the cell (" << row << ", " << col << ")" << std::endl;  
    }

    m_exitCoords[0] = row;
    m_exitCoords[1] = col;
}

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
Cell& Maze::findCell(int row, int col)
{
    if(row < 0 || row >= ROWCELLS || col < 0 || col >= COLCELLS)
    {
       std::cout << "ERROR: findCell() did not find the cell(" << row << ", " << col << ")" << std::endl;  
    }

    return m_cellsGrid[row][col];
}

/**--------------------------------------------------------------------------------------
 * getWallsMap()
 * 
 * Returns a reference to the map of walls in the maze
 * 
 * @return a reference to a Map of tuple<int, int, int, int> to Wall objects
 * --------------------------------------------------------------------------------------
*/
std::map<std::tuple<int, int, int, int>, Wall>& Maze::getWallsMap()
{
    return m_neighborsToWallMap;
}