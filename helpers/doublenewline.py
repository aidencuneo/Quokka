'''

Double newline checker

Run this script from the parent directory.

'''

import os
import string


def scan(path):
    if os.path.isdir(path) and not path.endswith('bin'):
        scandir(path)
    if not os.path.isfile(path) or not (
        path.endswith('.c') or path.endswith('.h')
    ):
        return
    with open(path) as f:
        d = f.read()
    last = ['', '']
    line = 1
    for i in range(len(d)):
        if d[i] == '\n':
            if last[0] == '\n' and last[1] == '\n':
                print('AT', path, 'LINE', line)
            line += 1
        last = [d[i], last[0]]


def scandir(path):
    for i in os.listdir(path):
        if i[0] != '.':
            scan(path + '/' + i)


scandir('.')
