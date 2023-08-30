# End-to-end maze generator, solver, and visualizer

## Project description
This program algorithmically generates a random maze of NxN dimensions and creates image files to visualize the maze in its solved and unsolved forms.

The maze generation is accomplished by implementing Wilson's algorithm in C++:
- Wilson's algorithm uses loop-erased random walks to generate a maze. This algorithm avoids biases towards long passageways or numerous dead ends that other algorithms such as DFS, Kruskal's, or Prim's algorithms have.

The maze solving is accomplished by implementing Tremaux's algorithm in C++:
- Tremaux's algorithm traverses the maze, and marks the exit taken at each junction. The algorithm iteratively backtracks and proceeds through the maze depending on the number of marks at each junction it walks through and the dead ends it encounters. It is gauranteed to work for all mazes with well-defined passageways.

The image generation is implemented with Python:
- The completed maze data is written to a CSV file by C++.
- A Python script then reads the CSV file and uses it to generate SVG files visualizing the solved and unsolved maze.

Legend for the maze:
- ![#FF7F50](https://placehold.co/15x15/FF7F50/FF7F50.png) `Entrance`
- ![#FF0000](https://placehold.co/15x15/FF0000/FF0000.png) `Exit`
- ![#90EE90](https://placehold.co/15x15/90EE90/90EE90.png) `Path`

Randomly generated maze             |  Same maze with path to exit
:-------------------------:|:-------------------------:
![](https://github.com/efei36/maze-generator-solver-visualizer/blob/main/mazeImageExamples/maze_20230828-19-04-42.svg)  |  ![](https://github.com/efei36/maze-generator-solver-visualizer/blob/main/mazeImageExamples/maze_with_exit_path_20230828-19-04-42.svg)

## Status
This program has been run and tested with the following. You will need these to use this program:
```
C++17 and newer
Python 3.10.11 and newer
g++ 12.2.0 and newer
If using an IDE, preferably VSCode version 1.81.1 and newer
Windows 11
```

## Instructions
### 1. How to compile
- Download all header files, source files, and python scripts into a folder of your choice, e.g., `<maze-folder>`.
- Compile the source files using a C++ compiler.
    - To compile from VSCode:
        - Download the `VSCodeJSONconfigs` folder, rename it to `.vscode`, and put it in your `<maze-folder>`.
        - Open `<maze-folder>` in VSCode, then compile `main.cpp` and all other source files. This will generate a `main.exe` executable.
        - For an in-depth explanation on how to use VSCode with C++, please refer to [here](https://code.visualstudio.com/docs/languages/cpp).
    - To compile from command line:
        - Replace `<maze-folder>` and `<your-excutable>` with your choices, and run the following command:<br />
            `maze-folder>g++  -fdiagnostics-color=always <maze-folder>/*.cpp -o <maze-folder>\<your-executable>.exe`
        - To access debugging statements, run the following command:<br />
            `maze-folder>g++ -DDO_DEBUG -fdiagnostics-color=always <maze-folder>/*.cpp -o <maze-folder>\<your-executable>.exe`
### 2. How to run
- Make sure that both python scripts `maze_img_displayer.py` and `run_all.py` are downloaded in your `<maze-folder>`.
- cd to your `<maze-folder>`:<br />
    `cd <maze-folder>`
- Run the following command:<br />
    `maze-folder>python3 run_all.py <your choice of side length>`
    - `run_all.py` takes a user specified side length "N" as an argument. It will first create a random NxN maze, then find a path from its entrance to its exit, and finally generate images visualizing the maze.
    - For example, to create, solve, and visualize a 30x30 maze, run the following in the command line:<br />
        `maze-folder>python3 run_all.py 30`
- Two SVG files with similar names to the following will be created in your `<maze-folder>`:
    ```
    maze-folder>maze_20230828-19-04-42.svg
    maze-folder>maze_with_exit_path_20230828-19-04-42.svg
    ```
- To view the svg files, open them in any SVG viewer, such as a Web brower.

## Possible future steps:
- Allow mazes with different shapes, e.g., triangles, circles, etc.
- Allow users to upload any maze. The program will scan it and find a path from its entrance to its exit.

## References
Wilson's algorithm: https://en.wikipedia.org/wiki/Maze_generation_algorithm<br />
Tremaux's algorithm: https://en.wikipedia.org/wiki/Maze-solving_algorithm<br />
SVG file creation: https://scipython.com/blog/making-a-maze/<br />