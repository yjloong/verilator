#!/bin/bash
gcc -shared -fPIC -o file_filter.so file_filter.c -ldl

# usage:
#LD_PRELOAD=./file_filter.so your_program