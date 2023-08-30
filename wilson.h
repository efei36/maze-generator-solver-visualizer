/*wilson.h*/

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

#pragma once

#include "maze.h"

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
void runWilson(Maze& blankMaze);