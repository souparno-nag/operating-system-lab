#!/bin/bash

echo "Enter a filename: "
read fname

case $fname in

*.sh)
echo "It is a bash script"
;;

*.jpg|*.png)
echo "It is an image file"
;;

*.txt)
echo "It is a text file"
;;

esac
