#!/bin/bash

gcc src/quokka.c -o bin/quokka
if [[ $? != 0 ]]; then exit; fi
bin/quokka $@
