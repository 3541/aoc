with open("data") as f:
    input = f.read().splitlines()

count = 0
for line in input:
    output = line.split(" | ")[1]
    for o in output.split():
        if len(o) in [2, 3, 4, 7]:
            count += 1

print(count)

def signal_union(s1, s2):
    return sorted(list(set(s1).union(set(s2))))

def signal_intersection(s1, s2):
    return sorted(list(set(s1).intersection(set(s2))))

def sort_all(l):
    for i in range(len(l)):
        l[i] = sorted(l[i])
    return l

sum = 0
for line in input:
    digits = [""] * 10
    [signals, outputs] = map(lambda o: sort_all(o.split()), line.split(" | "))
    for signal in signals:
        if len(signal) == 2:
            digits[1] = signal
        elif len(signal) == 3:
            digits[7] = signal
        elif len(signal) == 4:
            digits[4] = signal
        elif len(signal) == 7:
            digits[8] = signal
    signals = list(filter(lambda s: len(s) not in [2, 3, 4, 7], signals))
    digits[2] = filter(lambda s: len(s) == 5 and len(signal_intersection(s, digits[4])) == 2, signals)[0]
    digits[5] = filter(lambda s: len(s) == 5 and len(signal_intersection(s, digits[2])) == 3, signals)[0]
    digits[3] = filter(lambda s: len(s) == 5 and s != digits[5] and s != digits[2], signals)[0]

    digits[6] = filter(lambda s: len(s) == 6 and len(signal_intersection(s, digits[1])) == 1, signals)[0]
    digits[9] = filter(lambda s: len(s) == 6 and signal_intersection(s, digits[6]) == digits[5], signals)[0]
    digits[0] = filter(lambda s: s not in digits, signals)[0]

    rdigits = dict()
    for i in range(len(digits)):
        rdigits["".join(digits[i])] = i

    num = 0
    for signal in outputs:
        num *= 10
        num += rdigits["".join(signal)]
    sum += num
print(sum)
