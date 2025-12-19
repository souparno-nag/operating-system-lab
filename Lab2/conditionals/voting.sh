#!/bin/bash

echo "Enter your age"
read age
if [ "$age" -ge 18 ]
then
echo "Congratulations, you are eligible"
else
echo "You are not eligible to vote"
fi
