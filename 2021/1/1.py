with open("data") as f:
    input = f.read().splitlines()

prev = int(input[0])
up = 0
for line in input[1:]:
    if int(line) > prev:
        up += 1
    prev = int(line)

print(up)

window = [0] * len(input)
lines = input
for i in range(len(lines)):
    n = int(lines[i])
    window[i] = n
    if i > 0:
        window[i - 1] += n
    if i > 1:
        window[i - 2] += n

up = 0
prev = window[0]
for w in window:
    if w > prev:
        up += 1
    prev = w

print(up)
