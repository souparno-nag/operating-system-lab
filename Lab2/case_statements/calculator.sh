#!/bin/bash

echo -n "Enter 1 for addition, 2 for subtraction and 3 for multiplication: "
read choice

echo -n "Enter the two numbers: "
read a b

case $choice in

1)
sum=$((a+b))
echo "Sum=$sum"
;;

2)
diff=$((a-b))
echo "Difference=$diff"
;;

3)
prod=$((a*b))
echo "Product=$prod"
;;

4)
echo "Invalid choice"
;;

esac
