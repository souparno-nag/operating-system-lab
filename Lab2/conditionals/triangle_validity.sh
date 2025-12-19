#!/bin/bash

echo -n "Enter the three angles: "
read a b c

if (( a + b + c == 180 )); then
echo "This is a valid triangle"
else
echo "This is not a valid triangle"
fi
