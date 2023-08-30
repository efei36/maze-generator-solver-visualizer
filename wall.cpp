/*wall.cpp*/

/**
 * Author: Eric Fei
 * Version 0.0.1
 * 
 * Wall class
 * 
 * Defines a wall in a maze, which is in between two different cells
 * 
 * Is either a vertical wall or horizontal wall
 *     If vertical, then the cell to the left of the wall is recorded first as "Cell A"
 *     If horizontal, then the cell on top of the wall is recorded first as "Cell A"
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

#include "wall.h"

/**--------------------------------------------------------------------------------------
 * Constructor
 * 
 * Creates a wall between the two cells whose indices are given
 * 
 * The ints representing the indices of the cells are ordered as such:
 *     first pair of ints are the row and col of the "first" cell
 *     second pair of ints are the row and col of the "second" cell
 *     Order of cells is from left->right, top->bottom
 * 
 * @param[in] aR Row index of the first cell
 * @param[in] aC Column index of the first cell
 * @param[in] bR Row index of the second cell
 * @param[in] bC Column index of the second cell
 * --------------------------------------------------------------------------------------
*/
Wall::Wall(int aR, int aC, int bR, int bC)
  : m_cellARow(aR), m_cellACol(aC), m_cellBRow(bR), m_cellBCol(bC)
{}