#!/bin/bash

#Exception handling - making sure correct num of args provided
if [[ $# -lt 1 || $# -gt 1 ]]; then
	echo "You have provided the incorrect number of arguments. You need 1."
	exit 1
fi

#Assigning argument to a variable
fileName=$1

#Exception handling - Checking the file exists
if [[ ! -e "$fileName" ]]; then
	echo "No such file exists."
	exit 2
fi

#Sorting the file, then replacing the original data with the sorted
#data in the same file
sort -k1 -n -o $fileName $fileName

#Successful exit
exit 0
