with open("data") as f:
    input = f.read().splitlines()

depth = 0
x = 0

for line in input:
    [command, delta] = line.split(' ')
    delta = int(delta)
    if command == "forward":
        x += delta
    elif command == "down":
        depth += delta
    elif command == "up":
        depth -= delta

print(str(x * depth))

aim = 0
depth = 0
x = 0

for line in input:
    [command, delta] = line.split(' ')
    delta = int(delta)
    if command == "down":
        aim += delta
    elif command == "up":
        aim -= delta
    elif command == "forward":
        x += delta
        depth += aim * delta

print(str(x * depth))
