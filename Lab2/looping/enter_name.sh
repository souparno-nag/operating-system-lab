#!/bin/bash

name=""

while [[ "$name" != "quit" ]]; do
echo -n "Input your name (input 'quit' to exit): "
read name

if [[ "$name" == "quit" ]]; then
echo "Exiting.."
else
echo "Hello, $name!"
fi

done
