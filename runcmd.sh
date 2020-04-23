#!/bin/bash

gcc compiler.c -o tc
./tc $1 output.c
gcc output.c
./a.out $2 $3 $4 $5
rm a.out
