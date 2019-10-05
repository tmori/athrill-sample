#!/bin/bash

make -f Makefile.mbed $1
make $1
make -f Makefile.mros $1
make $1
