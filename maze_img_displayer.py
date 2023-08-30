"""maze_img_displayer.py"""

"""
Author: Eric Fei
Version 0.0.1

Modified code from below source:
Create a maze using the depth-first algorithm described at
https://scipython.com/blog/making-a-maze/
Christian Hill, April 2017.

Given the solved maze data from main.cpp, displays the unsolved and solved 
versions of the maze as separate png image files
"""

"""
MIT License
Copyright (c) 2023 Eric Fei
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import csv
import time

arr = []
total_rows = 0
total_cols = 0

with open('mazeData.csv', mode='r') as csv_file:
    """Opens the csv file containing the solved maze data, and reads it into a 2D array"""
    csv_reader = csv.reader(csv_file)
    for row_num, row in enumerate(csv_reader):
        if row_num == 0:
            total_rows = int(row[1])
            total_cols = int(row[0])
            arr = [["" for i in range(total_cols)] for j in range(total_rows)]
        else:
            for col_num, col_entry in enumerate(row[:-1]):
                arr[row_num - 1][col_num] = col_entry

def make_svg(file_name, maze_rows, maze_cols, maze_arr, bool_draw_path):
    """
    Using the solved maze data, creates a visual representation of the maze as an svg file.

    Arguments:
    file_name -- name of the svg file to be created
    maze_rows -- number of rows in the maze
    maze_cols -- number of columns in the maze
    maze_arr -- 2D array containing the solved maze data
    bool_draw_path -- controls whether or not the svg image will have the path from entrance to exit
    """
    aspect_ratio = maze_cols / maze_rows
    padding = 10

    height = 800
    width= int(height * aspect_ratio)

    scale_x = width / maze_cols
    scale_y = height / maze_rows

    def make_wall(w_f, w_x1, w_y1, w_x2, w_y2):
        print('<line x1="{}" y1="{}" x2="{}" y2="{}"/>'.format(w_x1, w_y1, w_x2, w_y2), file=w_f)

    def make_square(w_f, w_x, w_y, w_side_len, w_color):
        print('<rect x="{}" y="{}" width="{}" height="{}" stroke="{}" fill="{}" stroke-width="5"/>'.format(w_x, w_y, w_side_len, w_side_len, w_color, w_color), file=w_f)

    with open(file_name, 'x') as f:
        # SVG preamble and styles.
        print('<?xml version="1.0" encoding="utf-8"?>', file=f)
        print('<svg xmlns="http://www.w3.org/2000/svg"', file=f)
        print('    xmlns:xlink="http://www.w3.org/1999/xlink"', file=f)
        print('    width="{:d}" height="{:d}" viewBox="{} {} {} {}" style="background: white">'.format(width + padding * 2, height + padding * 2, -padding, -padding, width + padding * 2, height + padding * 2), file=f)
        print('<defs>\n<style type="text/css"><![CDATA[', file=f)
        print('line {', file=f)
        print('    stroke: #000000;\n    stroke-linecap: square;', file=f)
        print('    stroke-width: 5;\n}', file=f)
        print(']]></style>\n</defs>', file=f)

        # Drawing the squares and paths first, so that the maze walls are overlaid on top
        for row in range(maze_rows):
            for col in range(maze_cols):
                cur_cell_str = maze_arr[row][col]
                len_cur_str = len(cur_cell_str)

                # Drawing entrance and exit squares
                if cur_cell_str[:8] == "CellExit":
                    x = col * scale_x + scale_x * 0.05
                    y = row * scale_y + scale_x * 0.05
                    make_square(f, x, y, scale_x * 0.9, "red")
                elif len_cur_str > 11:
                    if cur_cell_str[:12] == "CellEntrance":
                        x = col * scale_x + scale_x * 0.05
                        y = row * scale_y + scale_x * 0.05
                        make_square(f, x, y, scale_x * 0.9, "coral")

                if bool_draw_path: # Drawing path from entrance to exit
                    if cur_cell_str[:8] == "CellPath":
                        x = col * scale_x + scale_x * 0.05
                        y = row * scale_y + scale_x * 0.05
                        make_square(f, x, y, scale_x * 0.9, "lightgreen")
        
        # Progressing through the maze row by row, from top to bottom:
        #     Progressing through each row column by column, from left to right:
        for row in range(maze_rows):
            for col in range(maze_cols):
                cur_cell_str = maze_arr[row][col]
                len_cur_str = len(cur_cell_str)

                # Drawing walls
                if cur_cell_str[len_cur_str - 2:] == "SE": # Southern and Eastern walls
                    # Making Southern wall
                    x1 = col * scale_x
                    x2 = (col + 1) * scale_x
                    y1 = (row + 1) * scale_y
                    y2 = (row + 1) * scale_y
                    make_wall(f, x1, y1, x2, y2)

                    # Making Eastern wall
                    x1 = (col + 1) * scale_x
                    x2 = (col + 1) * scale_x
                    y1 = row * scale_y
                    y2 = (row + 1) * scale_y
                    make_wall(f, x1, y1, x2, y2)
                elif cur_cell_str[len_cur_str - 1:] == "S": # Southern wall
                    x1 = col * scale_x
                    x2 = (col + 1) * scale_x
                    y1 = (row + 1) * scale_y
                    y2 = (row + 1) * scale_y
                    make_wall(f, x1, y1, x2, y2)
                elif cur_cell_str[len_cur_str - 1:] == "E": # Eastern wall
                    x1 = (col + 1) * scale_x
                    x2 = (col + 1) * scale_x
                    y1 = row * scale_y
                    y2 = (row + 1) * scale_y
                    make_wall(f, x1, y1, x2, y2)

        # Draw maze borders
        print('<line x1="0" y1="0" x2="{}" y2="0"/>'.format(width), file=f) # North border
        print('<line x1="0" y1="{}" x2="{}" y2="{}"/>'.format(height, width, height), file=f) # South border
        print('<line x1="{}" y1="0" x2="{}" y2="{}"/>'.format(width, width, height), file=f) # East border
        print('<line x1="0" y1="0" x2="0" y2="{}"/>'.format(height), file=f) # West border
        print('</svg>', file=f)

unsolved_fname = "maze_"
solved_fname = "maze_with_exit_path_"

timestamp = time.strftime("%Y%m%d-%H-%M-%S")

unsolved_fname += (timestamp + ".svg")
solved_fname += (timestamp + ".svg")

# Drawing the unsolved maze
make_svg(file_name = unsolved_fname, maze_rows = total_rows, maze_cols = total_cols, maze_arr = arr, bool_draw_path = False)

# Drawing the solved maze
make_svg(file_name = solved_fname, maze_rows = total_rows, maze_cols = total_cols, maze_arr = arr, bool_draw_path = True)