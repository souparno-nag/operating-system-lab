#!/bin/bash

echo -n "Enter traffic colour (red, yellow oe green): "
read colour

case $colour in

red)
echo "Stop"
;;

yellow)
echo "Wait"
;;

green)
echo "Go"
;;

esac
