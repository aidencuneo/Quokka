#!/bin/bash

gcc src/compiler.c -o bin/tc
bin/tc $1 examples/_temp.c
gcc examples/_temp.c -o examples/a.out
examples/a.out $2 $3 $4 $5
rm examples/a.out
