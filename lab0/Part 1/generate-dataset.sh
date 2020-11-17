#!/bin/bash

#Exception handling - making sure correct num of args provided
if [[ $# -lt 2 || $# -gt 2 ]]; then
	echo "You have provided the incorrect number of arguments. You need 2."
	exit 1
fi

#Assigning both args to variables
fileName=$1
numRecords=$2

#Creating temporary output file for later use
touch ints.txt

#Inserting 2 random integers per line, then outputting to temp file
for i in $(seq 1 $numRecords); do
	echo $RANDOM $RANDOM
done > ints.txt

#Generating proper number of random 100-byte ASCII strings, one per line
#and outputting them to another temporary file
base64 -w 100 /dev/urandom | head -n $numRecords > str.txt

#Combining the two temporary files by columns,
#Resulting in one file with 2 integers and 1 string per line
paste ints.txt str.txt > $fileName.txt

#Deleting the temporary files
rm ints.txt
rm str.txt

#Exiting the program
exit 0
