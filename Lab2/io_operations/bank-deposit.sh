#!/bin/bash

current=1000
echo "Current Balance: $current"
echo "Enter how much money to deposit: "
read deposit
updated=$((current+deposit))
echo "New balance: $updated"
