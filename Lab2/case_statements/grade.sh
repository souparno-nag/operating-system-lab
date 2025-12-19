#!/bin/bash

echo "Enter a grade: "
read grade

case $grade in

A)
echo "Excellent"
;;

B)
echo "Good"
;;

C)
echo "Average"
;;


D)
echo "Pass"
;;

F)
echo "Fail"
;;

esac
