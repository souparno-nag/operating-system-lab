#!/bin/bash

echo -n "Enter a number: "
read n

for ((i=0;i<10;i++))
do
echo "$n x $i = $((n*i))"
done
