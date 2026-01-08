#!/bin/bash

echo -n "Enter a number n: "
read n
sum=0

for ((i=0;i<=$n;i++))
do
sum=$((sum+i))
done

echo "The sum of first $n numbers is $sum"
