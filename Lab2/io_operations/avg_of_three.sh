#!/bin/bash

echo -n "Enter three numbers: "
read a b c

avg=$(((a+b+c)/3))
echo "The average of the three numbers is $avg"
