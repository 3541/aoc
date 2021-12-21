p1 = [8, 0]
p2 = [4, 0]

rolled = 0
i = 0
def roll():
    global i
    global rolled
    i %= 100
    i += 1
    rolled += 1
    return i

def play(player):
    move = roll() + roll() + roll()
    player[0] = (player[0] - 1 + move) % 10 + 1
    player[1] += player[0]

while p1[1] < 1000 and p2[1] < 1000:
    play(p1)
    if (p1[1] >= 1000):
        break
    play(p2)

losing_score = min(p1[1], p2[1])
print(losing_score * rolled)
