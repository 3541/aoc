grid = []
with open("data") as f:
    for line in f.read().splitlines():
        row = []
        for cell in line:
            row.append(int(cell))
        grid.append(row)

deltas = [
    [-1, 0],
    [0, 1],
    [1, 0],
    [0, -1]
]

def low_point(grid, row, col):
    cell = grid[row][col]
    for [dr, dc] in deltas:
        r = row + dr
        c = col + dc
        if 0 <= r and r < len(grid) and 0 <= c and c < len(grid[r]):
            if grid[r][c] <= cell:
                return False
    return True

def find_basin(grid, row, col):
    if (low_point(grid, row, col)):
        return [row, col]
    lowest = [row, col]
    lowest_point = grid[row][col]
    for [dr, dc] in deltas:
        r = row + dr
        c = col + dc
        if 0 <= r and r < len(grid) and 0 <= c and c < len(grid[r]):
            if grid[r][c] < lowest_point:
                lowest = [r, c]
                lowest_point = grid[r][c]
    return find_basin(grid, lowest[0], lowest[1])

sum = 0
basins = dict()
for i in range(len(grid)):
    for j in range(len(grid[i])):
        if low_point(grid, i, j):
            sum += grid[i][j] + 1
        if grid[i][j] != 9:
            [lr, lc] = find_basin(grid, i, j)
            key = str(lr) + "_" + str(lc)
            basins[key] = basins.get(key, 0) + 1
print(sum)

basin_sizes = list(reversed(sorted(basins.values())))
prod = 1
for basin in basin_sizes[:3]:
    prod *= basin
print(prod)
