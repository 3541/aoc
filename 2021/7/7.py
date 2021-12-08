import statistics

with open("data") as f:
    input = list(map(lambda n: int(n), f.read().split(',')))

m = int(statistics.median(input))

def dist_to(pos, crabs):
    dist = 0
    for crab in crabs:
        dist += abs(pos - crab)
    return dist

def fuel_to(pos, crabs):
    fuel = 0
    for crab in crabs:
        dist = abs(pos - crab)
        fuel += dist * (dist + 1) / 2
    return fuel

print(dist_to(m, input))

best_pos = 0
dist = fuel_to(0, input)
for i in range(len(input)):
    d = fuel_to(i, input)
    if d < dist:
        best_pos = i
        dist = d

print(dist)
