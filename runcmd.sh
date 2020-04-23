#!/bin/bash

gcc compiler.c -o compiler
./compiler $1 output.c
gcc output.c
./a.out
rm a.out
