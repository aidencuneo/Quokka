#!/bin/bash

gcc src/compiler.c -o bin/tc
bin/tc $1 examples/_temp.c $2
gcc examples/_temp.c -o bin/_temp.out
bin/_temp.out $3 $4 $5 $6 $7 $8 $9
