#!/bin/bash

echo "Enter three numbers:"
read a b c

if [ $a -gt $b ] && [ $a -gt $c ]; then
echo "$a is the greatest"
elif [ $c -gt $b ] && [ $c -gt $a ]; then
echo "$c is the greatest"
else
echo "$b is the greatest"
fi
