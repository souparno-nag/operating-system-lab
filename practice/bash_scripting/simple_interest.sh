#!/bin/bash

echo  "Enter principle, rate and number of years:"
read p r y

si=$(((p*r*y)/100))
echo "The simple interest is $si"
