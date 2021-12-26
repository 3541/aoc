from z3 import *

with open("data") as f:
    program = f.read().splitlines()

def reg(s):
    if s == "w":
        return 0
    elif s == "x":
        return 1
    elif s == "y":
        return 2
    else:
        return 3

def build_model(program):
    o = Optimize()

    registers = [BitVecVal(0, 64) for _ in range(4)]
    digits = [BitVec(i, 64) for i in range(14)]
    digit = 0

    for line in program:
        parts = line.split(" ")
        op = parts[0]
        dest = reg(parts[1])

        if op == "inp":
            registers[dest] = digits[digit]
            digit += 1
        else:
            operand = parts[2]
            if operand in ["w", "x", "y", "z"]:
                operand = registers[reg(operand)]
            else:
                operand = BitVecVal(int(operand), 64)

            if op == "add":
                registers[dest] += operand
            elif op == "mul":
                registers[dest] *= operand
            elif op == "div":
                registers[dest] /= operand
            elif op == "mod":
                registers[dest] %= operand
            elif op == "eql":
                registers[dest] = If(registers[dest] == operand, BitVecVal(1, 64), BitVecVal(0, 64))
            else:
                raise

    o.add(registers[reg("z")] == BitVecVal(0, 64))
    o.add([And(UGT(d, 0), ULT(d, 10)) for d in digits])
    return [o, digits]

def read_output(o, digits):
    o.check()
    m = o.model()

    ret = 0
    for d in digits:
        ret *= 10
        ret += m.evaluate(d).as_long()

    return ret

[o, digits] = build_model(program)
for d in digits:
    o.maximize(d)

print(read_output(o, digits))

[o, digits] = build_model(program)
for d in digits:
    o.minimize(d)

print(read_output(o, digits))
