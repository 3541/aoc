with open("test") as f:
    input = f.read().splitlines()

chars = {
    ')': '(',
    ']': '[',
    '}': '{',
    '>': '<'
}
opening = chars.values()
closing = chars.keys()

scores = {
    ')': 3,
    ']': 57,
    '}': 1197,
    '>': 25137
}

cscores = {
    '(': 1,
    '[': 2,
    '{': 3,
    '<': 4
}

score = 0
completion_scores = []
for line in input:
    stack = []
    corrupted = False
    for c in line:
        if c in opening:
            stack.append(c)
        else:
            l = stack.pop()
            if l != chars[c]:
                score += scores[c]
                corrupted = True
                break
    if not corrupted:
        s = 0
        for c in reversed(stack):
            s *= 5
            s += cscores[c]
        completion_scores.append(s)

print(score)
print(sorted(completion_scores)[len(completion_scores) / 2])
