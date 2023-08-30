/*main.cpp*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * main C++ file
 * 
 * Creates and solves the maze using Wilson's Algorithm and Tremaux's Algorithm
 * 
 * Writes resulting maze data to a .csv file
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
#include <fstream>
#include <string>
#include <stdlib.h>

#include "maze.h"
#include "wilson.h"
#include "tremaux.h"
#include "logger.h"

/**--------------------------------------------------------------------------------------
 * writeMazeDataCSV()
 * 
 * Write the completed and solved maze data to a csv file
 * 
 * @param[in,out]   outfile     Csv file to be modified, filled with solved maze information
 * @param[in]       solvedMaze  Maze object with path from entrance to exit
 * --------------------------------------------------------------------------------------
*/
void writeMazeDataCSV(std::fstream& outfile, Maze& solvedMaze)
{
    outfile << solvedMaze.getROWCELLS() << "," << solvedMaze.getCOLCELLS() << ",\n";
    std::tuple<int, int> entranceCoords = solvedMaze.getEntrance();
    int entranceRow = std::get<0>(entranceCoords);
    int entranceCol = std::get<1>(entranceCoords);

    std::tuple<int, int> exitCoords = solvedMaze.getExit();
    int exitRow = std::get<0>(exitCoords);
    int exitCol = std::get<1>(exitCoords);

    std::map<std::tuple<int, int, int, int>, Wall>& neighborsToWallsMap = solvedMaze.getWallsMap();

    for(int row = 0; row < solvedMaze.getROWCELLS(); row++)
    {
        std::string rowData = "";

        for(int col = 0; col < solvedMaze.getCOLCELLS(); col++)
        {
            // Cells
            if(row == entranceRow && col == entranceCol)
            {
                rowData += "CellEntrance";
            }
            else if(row == exitRow && col == exitCol)
            {
                rowData += "CellExit";
            }
            else if(solvedMaze.findCell(row, col).isCellOnPath())
            {
                rowData += "CellPath";
            }
            else
            {
                rowData += "CellRegular";
            }

            // Horizontal walls
            auto horizWallIter = neighborsToWallsMap.find(std::make_tuple(row, col, row + 1, col));
            if(horizWallIter != neighborsToWallsMap.end()){
                if(!horizWallIter->second.checkIsOpen()){ // South wall is closed
                    rowData += "S";
                }
            }

            // Vertical walls
            auto vertWallIter = neighborsToWallsMap.find(std::make_tuple(row, col, row, col + 1));
            if(vertWallIter != neighborsToWallsMap.end()){
                if(!vertWallIter->second.checkIsOpen()){  // East wall is closed
                    rowData += "E,";
                }
                else                                      // East wall is open
                {
                    rowData += ",";
                }
            }
        }
        
        rowData += ",";
        
        if(row < solvedMaze.getROWCELLS() - 1)
        {
            rowData += "\n";
        }

        outfile << rowData;
    }
}

/**--------------------------------------------------------------------------------------
 * printUsage()
 * 
 * Checks if the arguments is passed to main() are correct/usable
 * 
 * @param[in] argc Number of arguments passed
 * @param[in] argv String vector of arguments passed
 * @return true if the overall program should be terminated
 * --------------------------------------------------------------------------------------
*/
bool printUsage(int argc, const char** argv)
{
    bool shouldTerminate = false;
    int numRows = 0;
    if(argc != 2)
    {
        std::cerr << "ERROR: Incorrect number of arguments passed to main()" << std::endl;
        shouldTerminate = true;
    }
    else
    {
        numRows = atoi(argv[1]);

        if(numRows < 1)
        {
            std::cerr << "ERROR: Number of cells is too low" << std::endl;
            shouldTerminate = true;
        }
        else if(numRows < 3)
        {
            std::cout << "WARNING: Maze may be too small to be of value" << std::endl;
        }
        else if(numRows >= 100)
        {
            std::cout << "WARNING: Maze will be big, from here on out SVG quality may decrease and program may take a long time to complete" << std::endl;
        }
    }

    return shouldTerminate;
}

static int actualROWCELLS;
static int actualCOLCELLS;

int main(int argc, const char** argv)
{
    if(printUsage(argc, argv))
    {
        return -1;
    }

    actualROWCELLS= atoi(argv[1]);
    actualCOLCELLS = atoi(argv[1]);

    // Creating maze grid
    Maze mainMaze(actualROWCELLS, actualCOLCELLS);
    mainMaze.printMaze();

    // Running Wilson's Algorithm to fill out the maze
    runWilson(mainMaze);
    LOG_DEBUG("Wilson Finished")
    mainMaze.printMaze();

    // Running Tremaux's Algorithm to find a path from the entrance to the exit
    runTremaux(mainMaze);
    LOG_DEBUG("Tremaux Finished")

    LOG_DEBUG("Path from maze entrance to maze exit:")
    mainMaze.printMaze();
    
    // Writing finished maze data to a csv file
    std::fstream mazeData("mazeData.csv", std::fstream::out | std::fstream::trunc);
    writeMazeDataCSV(mazeData, mainMaze);
    mazeData.close();

    LOG_DEBUG("Program finished")

    return 0;
}