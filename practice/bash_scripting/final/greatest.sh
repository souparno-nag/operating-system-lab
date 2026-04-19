#!/bin/bash

read a b c
if [ $a -ge $b ] &&  [ $a -ge $c ]; then
echo "$a"
elif [ $b -ge $a ] && [ $b -ge $c ]; then
echo "$b"
elif [ $c -ge $b ] && [ $c -ge $a ]; then
echo "$c"
fi