from random import randint, randrange
import subprocess
from tqdm import trange, tqdm
from time import sleep, time
from math import exp, tanh
from random import random
import statistics

inf = 10000000.0

hw = 8
min_n_stones = 4 + 10

def digit(n, r):
    n = str(n)
    l = len(n)
    for i in range(r - l):
        n = '0' + n
    return n

def calc_n_stones(board):
    res = 0
    for elem in board:
        res += int(elem != '.')
    return res

evaluate = subprocess.Popen('../new_src/test/a.exe'.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
sleep(1)

n_phases = 30
n_scores = 129

def calc_stones(board):
    res = 0
    for i in board:
        if i != '.':
            res += 1
    return res

def collect_data(directory, num):
    global vhs, vds, vh_vd
    try:
        with open('data/' + directory + '/' + digit(num, 7) + '.txt', 'r') as f:
            data = list(f.read().splitlines())
    except:
        print('cannot open')
        return
    max_num = 5000
    print(len(data))
    for _ in tqdm(range(max_num)):
        datum = data[randrange(0, len(data))]
        #datum = data[randrange(0, len(data))]
        board, player, score = datum.split()
        score = float(score)
        if player == '1':
            score = -score
        n_stones = calc_n_stones(board)
        board_proc = player + '\n'
        for i in range(hw):
            for j in range(hw):
                board_proc += board[i * hw + j]
            board_proc += '\n'
        evaluate.stdin.write(board_proc.encode('utf-8'))
        evaluate.stdin.flush()
        v = float(evaluate.stdout.readline().decode().strip())
        with open('eval_error_data.txt', 'a') as f:
            f.write(str(n_stones) + ' ' + str(score) + ' ' + str(v) + '\n')

for i in range(20, 173):
    collect_data('records3', i)
    collect_data('records9', i)
evaluate.kill()
