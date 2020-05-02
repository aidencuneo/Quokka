#!/bin/bash

gcc src/compiler.c -o bin/tc -ggdb3
if [[ $? != 0 ]]; then exit; fi
bin/tc $1 examples/_temp.c $2
if [[ $? != 0 ]]; then exit; fi
gcc examples/_temp.c -o bin/_temp.out
if [[ $? != 0 ]]; then exit; fi
bin/_temp.out $3 $4 $5 $6 $7 $8 $9
