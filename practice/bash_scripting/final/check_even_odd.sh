#!/bin/bash

read n
rem=$((n%2))
if [ $rem -eq 1 ];
then
echo "Odd"
else
echo "Even"
fi
