from random import shuffle
from numpy import transpose
from time import time
from sys import argv
import math
import os

# File that will be opened
toOpen = "sudoku.txt"
# List representing special cells in the Sudoku grid
special_cells = []
# Size of the Sudoku grid (number of rows/columns)
sudokuSize = 0
# Size of a square region in the Sudoku grid
square = int(math.sqrt(sudokuSize))
# Set to store visited states of the Sudoku puzzle
visited = set()
# List representing the initial state of the Sudoku puzzle
sudokuMap = []
# List representing the current state of the Sudoku puzzle
data = []


# Initializes the Sudoku puzzle by randomly assigning numbers to empty cells
# while ensuring they don't conflict with existing numbers.
def initialization():
    for element_row in range(0, sudokuSize):
        fillment = []
        i = 1
        while i <= sudokuSize:
            if i not in sudokuMap[element_row]:
                fillment.append(i)
            i += 1
        shuffle(fillment)
        temp_fillment = []
        element_i = 0
        while element_i < sudokuSize:
            if not special_cells[element_row][element_i]:
                temp_fillment.append(fillment.pop())
            else:
                temp_fillment.append(data[element_row][element_i])
            element_i += 1

        element = 0
        while element < sudokuSize:
            data[element_row][element] = temp_fillment[element]
            element += 1

# Counts the number of conflicts (repeated numbers) in
# rows, columns, and squares of the Sudoku puzzle.
def calculate_conflicts():
    amount = 0

    # used numpy to transpose our grid
    columns = transpose(data)
    index = 0
    while index < len(columns):
        column = columns[index]
        amount += sudokuSize - len(set(column))
        index += 1

    for row in data:
        amount = amount + sudokuSize - len(set(row))

    element_i = 0
    while element_i < sudokuSize:
        element_j = 0
        while element_j < sudokuSize:
            square_info = []
            squared_i = element_i + square
            squared_j = element_j + square
            for x in range(element_i, squared_i):
                for y in range(element_j, squared_j):
                    square_info.append(data[x][y])
            amount += sudokuSize - len(set(square_info))

            element_j += square
        element_i += square

    return amount


# Counts the number of collisions (repeated numbers) in
# the given column for a specific number and row.
def count_collisions_in_column(number, column, row):
    collision_count = 1 - square

    element = 0
    while element < sudokuSize:
        if element != row:
            if data[element][column] == number:
                collision_count += 1
        element += 1

    return collision_count

# Counts the number of collisions (repeated numbers) in the square that
# contains the given row and column for a specific number.
def count_collisions_in_square(number, row, column):
    square_column = column // square
    square_row = row // square
    collision_count = 0

    element_first = 0
    while element_first < square:
        element_second = 0

        while element_second < square:

            current_row = square_row * square + element_first
            current_column = square_column * square + element_second

            if data[current_row][current_column] == number:
                if current_row != row or current_column != column:
                    collision_count += 1
            element_second += 1
        element_first += 1

    return collision_count

# Opens file from the path and converts it to the appropiate format for python.
def readFromFile():
    openedFile = open(toOpen, "r")

    global sudokuSize, square
    backslash = '\n'
    temporaryRow = openedFile.read().split(backslash)
    sudokuSize = int(temporaryRow[0])
    square = int(math.sqrt(sudokuSize))

    for element in temporaryRow[1::]:
        empty = ' '
        in_data = -1
        sudokuMap.append(list(map(int, element.split(empty))))
        data.append(list(map(int, element.split(empty))))
        special_cells.append(list(map(bool, data[in_data])))

    openedFile.close()


# Finds positions (rows, columns) in the Sudoku puzzle with the maximum number of collisions.
def find_positions_with_max_collisions():
    map_data = {}

    for element_row in range(sudokuSize):
        for element_column in range(sudokuSize):
            if special_cells[element_row][element_column]:
                continue
            collision_count = count_collisions_at_position(element_row, element_column)
            if collision_count not in map_data:
                map_data[collision_count] = [(element_row, element_column)]
            else:
                map_data[collision_count].append((element_row, element_column))

    max_collision_count = max(map_data)
    positions_with_max_collisions = map_data[max_collision_count]

    return max_collision_count, positions_with_max_collisions

# Performs the main iteration of the hill climbing algorithm, exploring neighboring
# states to reduce collisions and updating the best solution accordingly.
def climbMain(collisions, worst_band):
    rate, arrayPair = find_positions_with_max_collisions()
    amount = collisions
    neighbours = []
    additional_information = []

    for paired in arrayPair:
        num = paired[1]
        row = paired[0]
        for element in range(0, sudokuSize):
            # useless cases will be skipped
            if special_cells[row][element]:
                continue
            if element == num:
                continue

            data[row][num], data[row][element] = data[row][element], data[row][num]
            # add if not visited
            if str(data) in visited:
                data[row][num], data[row][element] = data[row][element], data[row][num]
                continue

            # added to visited
            visited.add(str(data))

            calculated = calculate_conflicts()

            if calculated < amount:
                amount = calculated
                neighbours = str(data)
                additional_information = []
            if calculated == amount:
                additional_information.append(str(data))

            data[row][num], data[row][element] = data[row][element], data[row][num]

    if amount != collisions:
        if additional_information:
            worst_band.clear()
    worst_band += additional_information

    return amount, neighbours

# Calculates the total number of collisions at a specific position (row, column) in the Sudoku puzzle.
def count_collisions_at_position(row, column):
    number = data[row][column]
    column_collision_count = count_collisions_in_column(number, column, row)
    square_collision_count = count_collisions_in_square(number, row, column)
    result_sum = column_collision_count + square_collision_count
    return result_sum


# Executes the hill climbing algorithm to solve the Sudoku puzzle by
# iteratively improving the current solution to reduce collisions.
def hill_climb():
    global visited, data
    bad_case = []
    issue_count = calculate_conflicts()
    visited.add(str(data))

    while issue_count:
        os.system('cls' if os.name == 'nt' else 'clear')
        outputTerminal(solved=False)
        issue_count, around = climbMain(issue_count, bad_case)
        if not around:
            if bad_case:
                parse_sudoku_string(bad_case.pop(-1))
            else:
                while True:
                    if str(data) not in visited:
                        visited.add(str(data))
                        break
                    initialization()
            issue_count = calculate_conflicts()
        else:
            parse_sudoku_string(around)
            os.system('cls' if os.name == 'nt' else 'clear')
            outputTerminal(solved=True)
            parse_sudoku_string(around)


# Ouput for showing how solving sudoku looks like in each step.
def outputTerminal(solved=True):
    if solved:
        print("Solved solution:")
    else:
        print("Current solution:")
    i = 0
    while i < sudokuSize:
        if i % square == 0:
            if i != 0:
                print(" " * (sudokuSize + square - 1))
        j = 0
        while j < sudokuSize:
            if j != 0:
                if j % square == 0:
                    print(" ", end='')
            print(data[i][j], '', end='')
            j += 1
        print()
        i += 1
    print(" " * (sudokuSize + square - 1))


# Parses a string representation of the Sudoku puzzle and stores it in the data variable.
def parse_sudoku_string(str_data):
    global data
    split_string = "], "
    str_data = str_data[1:-1:].split(split_string)
    str_data[-1] = str_data[-1][:-1:]

    splitted = []

    for str_data in str_data:
        tmp = ", "
        split_data = str_data[1:].split(tmp)
        splitted.append([int(item) for item in split_data])

    data = splitted


# Reads the initial Sudoku puzzle, initializes it, runs the hill climbing
# algorithm, and prints the time taken to solve the puzzle.
def result():
    global toOpen
    toOpen = argv[1]
    readFromFile()
    initialization()
    start = time()
    hill_climb()
    end = time()
    print("Solved, time for solving: " f"{end - start:.2f}")


# Main function
result()
