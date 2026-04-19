#!/bin/bash

x=$((RANDOM % 50 + 1))
echo -n "Enter a number: "
read y

while [ $y -ne $x ]
do
if [ $y -gt $x ]; then
echo "Guess lower"
elif [ $y -lt $x ]; then
echo "Guess higher"
fi
echo -n "Enter a number: "
read y
done
