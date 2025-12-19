#!/bin/bash

echo -n "Enter the filename: "
read fname

if [ -e $fname ]; then
	if [ -x $fname ]; then
		echo "The file is executable"
	else
		echo "The file is not executable"
	fi
else
	echo "File $fname does not exist"
fi
