'''

Trailing whitespace checker

Run this script from the parent directory.

'''

import os
import string


def scan(path):
    if os.path.isdir(path) and not path.endswith('bin'):
        scandir(path)
    if not os.path.isfile(path):
        return
    with open(path) as f:
        d = f.read()
    s = d.split('\n')
    for i in range(len(s)):
        if len(s[i]) and s[i][-1] in string.whitespace:
            print('AT', path, 'LINE', i + 1, ':', s[i])


def scandir(path):
    for i in os.listdir(path):
        if i[0] != '.' and i != 'valgrind-out.txt' and i != 'profile.txt':
            scan(path + '/' + i)


scandir('.')
