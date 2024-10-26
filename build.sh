#!/bin/bash

set -xe

LIBS=-L./

gcc -g -o gtodo $LIBS gtodo.c -lsqlite3 -l:libraylib.a -lm
