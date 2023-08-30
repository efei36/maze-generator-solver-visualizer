import subprocess
import sys

str_side_length = ""

# Checking if correct number of arguments passed
if len(sys.argv) != 2:
    print("ERROR: Incorrect number of arguments passed to run_all.py, please one side length")
    sys.exit(2)
else:
    str_side_length = sys.argv[1]

# Making sure inputted side length is valid
converted_length = float(str_side_length)
int_length = int(converted_length)
str_side_length = str(int_length)

if int_length < 1:
    print("ERROR: Side length is too low, please give a valid side length with value greater than 0")
    sys.exit(2)

if not converted_length.is_integer():
    print("User-inputted side length of", converted_length, "is not valid, rounding down to", int_length)

# Generating and solving the maze with C++
subprocess.run(["main.exe", str_side_length])

# Creating svg files of the solved and unsolved maze
subprocess.call(["python3", "maze_img_displayer.py"])