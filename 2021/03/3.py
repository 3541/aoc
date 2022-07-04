with open("data") as f:
    input = f.read().splitlines()

def count_freqs(l):
    f = [0] * len(input[0])

    for line in l:
        for i in range(len(line)):
            if line[i] == '0':
                f[i] -= 1
            else:
                f[i] += 1
    return f

freqs = count_freqs(input)

e = 0
g = 0
for f in freqs:
    g *= 2
    e *= 2
    if f >= 0:
        g += 1
    else:
        e += 1
print(g * e)

oc = input[:]
of = False
cc = input[:]
cf = False

for i in range(len(freqs)):
    ofr = count_freqs(oc)
    cfr = count_freqs(cc)

    if not of:
        if ofr[i] >= 0:
            oc = list(filter(lambda l: l[i] == "1", oc))
        else:
            oc = list(filter(lambda l: l[i] == "0", oc))
    if not cf:
        if cfr[i] >= 0:
            cc = list(filter(lambda l: l[i] == "0", cc))
        else:
            cc = list(filter(lambda l: l[i] == "1", cc))

    if len(oc) == 1:
        of = True
    if len(cc) == 1:
        cf = True

    if of and cf:
        break

o = 0
c = 0
for i in range(len(freqs)):
    o *= 2
    c *= 2
    o += int(oc[0][i])
    c += int(cc[0][i])

print(oc[0])
print(o)
print(c)
print(o * c)
