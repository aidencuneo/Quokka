#!/bin/bash

gcc -g -pg -ggdb3 src/quokka.c -o bin/quokka
if [[ $? != 0 ]]; then exit; fi
bin/quokka $1 $2 $3
if [[ $? != 0 ]]; then exit; fi

cat gmon.out > /dev/null
while [ $? != 0 ]; do
    printf "Move gmon.out into the main Quokka directory, press enter when complete... "
    read
    cat gmon.out > /dev/null
done

gprof bin/quokka > profile.txt
rm gmon.out
