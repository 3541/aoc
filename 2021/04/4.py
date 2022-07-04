with open("data") as f:
    input = f.read().splitlines()

draws = list(map(lambda d: int(d), input[0].split(",")))

n_boards = len(list(filter(lambda l: l != "", input[1:])))
boards = [[]]

i = 0
b = 0
for l in input[1:]:
    if l != "":
        boards[b].append(list(map(lambda x: int(x), l.split())))
    else:
        i = 0
        b += 1
        boards.append([])

boards = list(filter(lambda l: len(l) != 0, boards))

def complete(board):
    for row in board:
        complete = True
        for col in row:
            if col != -42:
                complete = False
                break
        if complete:
            return True
    for i in range(len(board[0])):
        complete = True
        for j in range(len(board)):
            if board[j][i] != -42:
                complete = False
                break
        if complete:
            return True
    return False

def run(boards, draws, p1=True):
    completed = []
    for draw in draws:
        for b in range(len(boards)):
            board = boards[b]
            for i in range(len(board)):
                for j in range(len(board[i])):
                    if board[i][j] == draw:
                        board[i][j] = -42
                if complete(board):
                    if p1 or len(completed) == len(boards):
                        sum = 0
                        if not p1:
                            board = completed[-1]
                        for row in board:
                            for col in row:
                                if col != -42:
                                    sum += col
                        print(sum * draw)
                        return
                    else:
                        if not board in completed:
                            completed.append(board)
run(boards, draws)
run(boards, draws, p1=False)
