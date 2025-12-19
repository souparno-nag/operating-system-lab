#!/bin/bash

echo -n "Enter an alphabet: "
read al

case $al in

a|e|i|o|u|A|E|I|O|U)
echo "$al is a vowel"
;;

*)
echo "$al is a consonant"
;;

esac
