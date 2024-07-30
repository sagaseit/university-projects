import heapq
import random
import time
import os

WALL = 'X'
OPENED = '#'
PATH = 'o'
START = 'S'
END = 'E'

dx = [-1, 1, 0, 0]
dy = [0, 0, -1, 1]

def load_and_parse_maze(file_path):
    with open(file_path, 'r') as file:
        maze_lines = file.readlines()
    maze = [list(line.strip()) for line in maze_lines[:-2]]
    start_coords = tuple(map(int, maze_lines[-2].split()[1].split(',')))
    end_coords = tuple(map(int, maze_lines[-1].split()[1].split(',')))
    start = (start_coords[1], start_coords[0])
    end = (end_coords[1], end_coords[0])
    return maze, start, end


def choose_algorithm():
    print("Choose an algorithm to run:")
    print("1. BFS")
    print("2. DFS")
    print("3. Random Search")
    print("4. Greedy Search")
    print("5. A*")

    while True:
        choice = input("Enter your choice: ")
        if choice in ['1', '2', '3', '4', '5']:
            return int(choice)
        else:
            print("Invalid input. Please enter a number between 1 and 5.")


class Node:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.left = None
        self.right = None
        self.up = None
        self.down = None


def bfs(maze, start, end):
    queue = [(start, [start])]
    visited = set()
    opened_nodes = 0
    path_nodes = 0

    while queue:
        (x, y), path = queue.pop(0)
        if (x, y) == end:
            for node_x, node_y in path:
                maze[node_x][node_y] = PATH
            start1, start2 = start
            end1, end2 = end
            maze[start1][start2] = START
            maze[end1][end2] = END
            path_nodes = len(path) - 1
            print_maze_with_info(maze, opened_nodes, path_nodes)
            print("Path length: ", path_nodes)
            break

        if (x, y) in visited:
            continue

        visited.add((x, y))

        for i in range(4):
            nx, ny = x + dx[i], y + dy[i]
            if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] != WALL:
                if (nx, ny) not in visited and (nx, ny) not in [item[0] for item in queue]:
                    queue.append(((nx, ny), path + [(nx, ny)]))
                    if maze[nx][ny] != END:
                        maze[nx][ny] = OPENED
                        opened_nodes += 1
                    print_maze_with_info(maze, opened_nodes, path_nodes)

    return False

def dfs(maze, start, end):
    stack = [(start, [start])]
    visited = set()
    opened_nodes = 0
    path_nodes = 0

    while stack:
        (x, y), path = stack.pop()
        if (x, y) == end:
            for node_x, node_y in path:
                maze[node_x][node_y] = PATH
            start1, start2 = start
            end1, end2 = end
            maze[start1][start2] = START
            maze[end1][end2] = END
            path_nodes = len(path) - 1
            print_maze_with_info(maze, opened_nodes, path_nodes)
            print("Path length: ", path_nodes)
            break

        if (x, y) in visited:
            continue

        visited.add((x, y))

        for i in range(4):
            nx, ny = x + dx[i], y + dy[i]
            if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] != WALL:
                if (nx, ny) not in visited:
                    stack.append(((nx, ny), path + [(nx, ny)]))
                    if maze[nx][ny] != END:
                        opened_nodes += 1
                        maze[nx][ny] = OPENED
                    print_maze_with_info(maze, opened_nodes, path_nodes)
    return False

def manhattan_distance(start, end):
    return abs(start[0] - end[0]) + abs(start[1] - end[1])

def greedy_search(maze, start, end):
    maze[start[0]][start[1]] = START
    maze[end[0]][end[1]] = END
    heap = [(manhattan_distance(start, end), start)]
    heapq.heapify(heap)
    prev = {}
    visited = set()
    opened_nodes = 0
    path_nodes = 0

    while heap:
        _, current = heapq.heappop(heap)
        if current == end:
            path = [current]
            while current in prev:
                current = prev[current]
                path.append(current)
                path_nodes += 1
            return opened_nodes, path_nodes, list(reversed(path))

        visited.add(current)
        for neighbor in neighbors(maze, current):
            if neighbor not in visited:
                priority = manhattan_distance(neighbor, end)
                heapq.heappush(heap, (priority, neighbor))
                prev[neighbor] = current
                opened_nodes += 1
                maze[neighbor[0]][neighbor[1]] = OPENED
                print_maze_with_info(maze, opened_nodes, path_nodes)

    return None

def neighbors(maze, current):
    x, y = current
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    valid_neighbors = []
    for dx, dy in directions:
        nx, ny = x + dx, y + dy
        if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] != WALL:
            valid_neighbors.append((nx, ny))
    return valid_neighbors

def reconstruct_path(prev, current, nodes):
    path = [current]
    while current in prev:
        current = prev[current]
        path.append(current)
        nodes += 1
    return list(reversed(path))

def a_star(maze, start, end):
    maze[start[0]][start[1]] = START
    maze[end[0]][end[1]] = END
    open_set = [(0, start)]
    heapq.heapify(open_set)
    prev = {}
    g_scores = {start: 0}
    visited = set()
    closed_set = set()
    opened_nodes = 0
    path_nodes = 0

    while open_set:
        _, current = heapq.heappop(open_set)

        if current == end:
            path = [current]
            while current in prev:
                current = prev[current]
                path.append(current)
                path_nodes = path_nodes + 1
            return opened_nodes, path_nodes, list(reversed(path))

        visited.add(current)

        for neighbor in neighbors(maze, current):
            new_g_score = g_scores[current] + 1

            if neighbor in closed_set:
                continue

            if neighbor not in g_scores or new_g_score < g_scores[neighbor]:
                g_scores[neighbor] = new_g_score
                f_score = new_g_score + manhattan_distance(neighbor, end)
                heapq.heappush(open_set, (f_score, neighbor))
                prev[neighbor] = current
                opened_nodes += 1
                maze[neighbor[0]][neighbor[1]] = OPENED

        closed_set.add(current)
        print_maze_with_info(maze, opened_nodes, path_nodes)

    return None


def random_search(maze, start, end):
    open_set = [start]
    prev = {}
    visited = set()
    opened_nodes = 1
    path_nodes = 0

    while open_set:
        x = random.choice(open_set)
        if x == end or end in visited:
            return opened_nodes, path_nodes, reconstruct_path(prev, x, path_nodes)

        visited.add(x)
        for neighbor in neighbors(maze, x):
            if neighbor not in visited and neighbor not in open_set:
                open_set.append(neighbor)
                prev[neighbor] = x
                opened_nodes += 1
                maze[neighbor[0]][neighbor[1]] = OPENED
            print_maze_with_info(maze, opened_nodes, path_nodes)

        open_set.remove(x)

    return None


def print_maze(maze):
    for row in maze:
        print(''.join(row))

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def print_maze_with_info(maze, opened_nodes, path_nodes):
    clear_screen()
    for row in maze:
        print(''.join(row))
    print("---------------------------------------")
    print("Number of opened nodes:", opened_nodes)
    time.sleep(0.5)


maze_str = "73.txt"

maze, start, end = load_and_parse_maze(maze_str)
choice = choose_algorithm()
opened_nodes = 0
path_nodes = 0
path = []

bfsORdfs = False
randomAlg = False

if choice == 1:
    print("Running BFS algorithm...")
    bfs(maze, start, end)
elif choice == 2:
    print("Running DFS algorithm...")
    dfs(maze, start, end)
elif choice == 3:
    print("Running Random Search algorithm...")
    opened_nodes, path_nodes, path = random_search(maze, start, end)
    randomAlg = True
elif choice == 4:
    print("Running Greedy Search algorithm...")
    opened_nodes, path_nodes, path = greedy_search(maze, start, end)
    bfsORdfs = True
elif choice == 5:
    print("Running A* algorithm...")
    opened_nodes, path_nodes, path = a_star(maze, start, end)
    bfsORdfs = True
else:
    print("Invalid choice. Exiting...")

if randomAlg:
    clear_screen()
    if path:
        for i, j in path:
            if (i, j) != start and (i, j) != end:
                maze[i][j] = PATH
                path_nodes += 1

        start_x, start_y = start
        end_x, end_y = end
        maze[start_x][start_y] = START
        maze[end_x][end_y] = END

        print("".join("".join(row) + "\n" for row in maze))
        print("---------------------------------------")
        print("Number of opened nodes:", opened_nodes)
        print("Number of nodes in the path:", path_nodes + 1)
    else:
        print("No path found.")

if bfsORdfs:
    clear_screen()
    if path:
        for i, j in path:
            if (i, j) != start and (i, j) != end:
                maze[i][j] = PATH

        start_x, start_y = start
        end_x, end_y = end
        maze[start_x][start_y] = START
        maze[end_x][end_y] = END

        print("".join("".join(row) + "\n" for row in maze))
        print("---------------------------------------")
        print("Number of opened nodes:", opened_nodes)
        print("Number of nodes in the path:", path_nodes)
    else:
        print("No path found.")
