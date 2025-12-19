#!/bin/bash

check=false
count=0

until $check
do
echo "Iteration no: $count"
((count++))
if [ $count == 11 ]; then
check=true
fi
done
