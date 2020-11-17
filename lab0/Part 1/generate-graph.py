#!/usr/bin/env python3

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

x = [1000,100000,10000000]
xlabels = ['1000','100000','10000000']

#Setting up data
real = []
user = []
sys = []
	
def generate_dataset(fileName):
	#Setting up global variables
	global real
	global user
	global sys
	
	#Extracting data from the file
	file = open(fileName)
	times = []
	for x in range(1,4):
		times.append(file.readline())
	file.close()
	
	#Organizing like data
	for i in range(len(times)):
		data = times[i].split(' ')
		real.append(float(data[0]))
		user.append(float(data[1]))
		sys.append(float(data[2]))
		
def generate_graph(str):
	
	#Creating a bar graph
	width = 0.3
	plt.bar(np.arange(len(real)), real, width=width,label='real')
	plt.bar(np.arange(len(user))+width, user, width=width)
	plt.bar(np.arange(len(sys))+width+width, sys, width=width)
	
	#Adding labels to axes
	plt.xlabel('Number of records')
	plt.ylabel('Time in seconds(s)')
	
	#Adjusting the axes to include all data
	plt.xticks(ticks=x,labels=xlabels)
	plt.yscale("log")
	plt.margins(0.2)
	
	#Setting title
	plt.title(str)
	
	#Show the graph
	plt.show()
	
	
def clear_data():
	#Setting up global variables
	global real
	global user
	global system
	
	#Clear data variables
	real.clear()
	user.clear()
	sys.clear()

		
def main():
	
	#Import generate-dataset.sh data
	generate_dataset("gen-data-times")
	#Graph generate-dataset.sh data
	generate_graph("generate-data Run Time")
	
	#Clear data variables
	clear_data()
	#Clear plot
	plt.clf()
	
	#Import sort-data.sh data
	generate_dataset("sort-data-times")
	#Graph sort-data.sh data
	generate_graph("sort-data Run Time")
	
#Run program	
main()
