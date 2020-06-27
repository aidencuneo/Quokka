'''

Replaces all tabs with four spaces
(go through log to fix replaced tabs)

Run this script from the parent directory.

'''

import os
import string


def scan(path):
    if os.path.isdir(path) and not path.endswith('bin'):
        scandir(path)
    if not os.path.isfile(path):
        return
    with open(path, 'r+') as f:
        d = f.read()
        i = 0
        while '\t' in d[i:]:
            i = d.index('\t', i)
            print('AT', path, end=' HERE : ')
            while True:
                print(end=d[i])
                if d[i] == '\n':
                    break
                i += 1
        if i:
            d = d.replace('\t', '    ')
            f.seek(0)
            f.write(d)


def scandir(path):
    for i in os.listdir(path):
        if i[0] != '.' and i != 'valgrind-out.txt' and i != 'profile.txt':
            scan(path + '/' + i)


scandir('.')
