#!/bin/bash

echo -n "Press 1 or 2: "
read ch

case $ch in
1) echo "Hello";;
2) echo "Bye"
echo "Bye"
;;
*) echo "Invalid"
esac
