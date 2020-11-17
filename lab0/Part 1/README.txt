CS 351 Lab #0 Part 1 - Linux & Tools

This is an introductory lab completed for CS 351 at IIT. The lab's purpose was to introduce students to command line functions with Linux through the use of VirtualBox. Part 1 of the lab asked that students set up two identical virtual machines, and test that they could successfully ssh from one machine to another. Part 2 of the lab was a detailed exploration of various terminal commands. Evidence of these trials were provided via screenshots, as well as short explanations of each command. Part 3 of the lab required writing bash scripts, as well as a Python matplotlib script. The scripts, their explanations, and examples are as follows:


generate-dataset.sh
	This bash script creates a randomized dataset consisting of two integers and one string per line. It takes two arguments - a file name and a number of records. It will create the correct number of records, then output them to the provided file.
Example: ./generate-dataset.sh example1.txt 1000
	In this example, the script will create 1,000 lines of random integers and strings, which will then be output to example1.txt.

sort-data.sh
	This bash script will sort a given dataset by the first column only. It takes only one argument - a file name. The script will sort the data, then overwrite the file with the sorted data.
Example: ./sort-data.sh example1.txt
	In this example, the file example1.txt will go into the script unsorted, and come out in ascending order by the first column.

generate-graph.py
	This Python script will generate one 3-bar graph for each script with its associated time readings. You will need two files - one for each bash script. Each file will have 3 rows of data, each with 3 columns. The 3 rows correspond with the runtimes of 1000, 100000, and 10000000 records. The 3 columns correspond with the 3 readings given - real, user, and sys. The script needs to be edited to read from those files within the main() function. It takes no arguments. Upon running, the program will produce the 2 bar graphs.
Example: ./generate-graph.py
	This example will produce 2 bar graphs from the data provided.


Part 4 of this lab consisted of answering basic questions relating to computer systems and how to setup VirtualBox accordingly. Answers were researched through the Internet, as well as with the VirtualBox manual.