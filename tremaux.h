/*tremaux.h*/

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

#pragma once

#include "maze.h"

#include <stack>

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
void runTremaux(Maze& unsolvedMaze);