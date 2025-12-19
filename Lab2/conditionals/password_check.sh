#!/bin/bash

password="abracadabra"
echo "Enter the password: "
read check
if [ "$password" = "$check" ]
then
echo "Correct password"
else
echo "Wrong password"
fi
