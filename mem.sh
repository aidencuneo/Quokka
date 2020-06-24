#!/bin/bash

# -ftrapv will generate SIGABRT on a signed integer overflow
gcc src/quokka.c -o bin/quokka -ggdb3 -ftrapv
if [[ $? != 0 ]]; then exit; fi
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file=valgrind-out.txt\
         bin/quokka $@
