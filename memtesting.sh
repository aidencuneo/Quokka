#!/bin/bash

gcc src/compiler.c -o bin/tc -ggdb3
if [[ $? != 0 ]]; then exit; fi
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt bin/tc $1 examples/_temp.c $2
