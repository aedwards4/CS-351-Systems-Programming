#!/bin/bash

# iterate over the program parameters using nested for loops
# program usage: ./fileio <workload> <block_size> <num_procs>
#     - workload: WS (write-sequential), WR (write-random), RS (read sequential), RR (read random)
#     - block_size: 64KB, 1MB, 64MB
#     - num_procs: 1 2 4 8


workload=(WS WR RS RR)
blockSize=(64 1024 16384)
numProcs=(1 2 4 8)


#All 1GB experiments
for n in ${numProcs[@]}; do
  for s in ${blockSize[@]}; do
    /home/alexis/Desktop/iozone3_490/src/current/iozone -s 1g -r ${s} -i 0 -i 1 -i 2 -t ${n}
  done
done


#All 4KB experiments
for n in ${numProcs[@]}; do
  /home/alexis/Desktop/iozone3_490/src/current/iozone -s 4 -r 4 -i 0 -i 1 -i 2 -t ${n} -O
done


#Exiting the program
exit 0
