'''

Line counter

Usage:
    py/python/python3 helpers/<scriptname.py> <Extensions>
Extension Format for C, C Headers, and Quokka Scripts:
    c,h,q

Run this script from the parent directory.

'''

import os
import string
import sys

# File extensions to count
CHECK_FOR = sys.argv[1].split(',') if len(sys.argv) > 1 else ['c', 'h']

linecount = 0


def count(path):
    global linecount
    if not os.path.isfile(path):
        return
    with open(path) as f:
        d = f.read()
    print(path)
    linecount += d.count('\n') + 1


def countdir(path):
    for i in os.listdir(path):
        if os.path.isdir(path + '/' + i) and not path.endswith('bin'):
            countdir(path + '/' + i)
        elif i.split('.')[-1] in CHECK_FOR:
            count(path + '/' + i)


countdir('.')
print()
print('-- Extensions --')
print(*['*.' + a for a in CHECK_FOR], sep='\n')
print()
print(linecount)
