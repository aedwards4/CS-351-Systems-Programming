#!/bin/bash

# iterate over the program parameters using nested for loops
# program usage: ./fileio <workload> <block_size> <num_procs>
#     - workload: WS (write-sequential), WR (write-random), RS (read sequential), RR (read random)
#     - block_size: 4KB, 64KB, 1MB, 64MB
#     - num_procs: 1 2 4 8

workload=(WS WR RS RR)
blockSize=(64KB 1MB 16MB)
numProcs=(1 2 4 8)

# right now the bash script calls the program with only one configuration
#./fileio $workload $block_size $num_procs

#4MB datasets
#D1
fallocate -x -l 4M file4mb
#D2
fallocate -x -l 2M file2mb-0
fallocate -x -l 2M file2mb-1
#D3
fallocate -x -l 1M file1mb-0
fallocate -x -l 1M file1mb-1
fallocate -x -l 1M file1mb-2
fallocate -x -l 1M file1mb-3
#D4
fallocate -x -l 500K file500kb-0
fallocate -x -l 500K file500kb-1
fallocate -x -l 500K file500kb-2
fallocate -x -l 500K file500kb-3
fallocate -x -l 500K file500kb-4
fallocate -x -l 500K file500kb-5
fallocate -x -l 500K file500kb-6
fallocate -x -l 500K file500kb-7


#1GB datasets
#D5
fallocate -x -l 1G file1gb
#D6
fallocate -x -l 500M file500-0
fallocate -x -l 500M file500-1
#D7
fallocate -x -l 250M file250-0
fallocate -x -l 250M file250-1
fallocate -x -l 250M file250-2
fallocate -x -l 250M file250-3
#D8
fallocate -x -l 125M file125-0
fallocate -x -l 125M file125-1
fallocate -x -l 125M file125-2
fallocate -x -l 125M file125-3
fallocate -x -l 125M file125-4
fallocate -x -l 125M file125-5
fallocate -x -l 125M file125-6
fallocate -x -l 125M file125-7

#All 1GB experiments
for w in ${workload[@]}; do
  for n in ${numProcs[@]}; do
    for s in ${blockSize[@]}; do
      ./fileio ${w} ${s} ${n}
    done
  done
done

newWorkload=(WR RR)

#All 4KB experiments
for w in ${newWorkload[@]}; do
  for n in ${numProcs[@]}; do
      ./fileio ${w} 4KB ${n} -O
  done
done

#4MB datasets
#D1
rm file4mb
#D2
rm file2mb-0
rm file2mb-1
#D3
rm file1mb-0
rm file1mb-1
rm file1mb-2
rm file1mb-3
#D4
rm file500kb-0
rm file500kb-1
rm file500kb-2
rm file500kb-3
rm file500kb-4
rm file500kb-5
rm file500kb-6
rm file500kb-7


#1GB datasets
#D5
rm file1gb
#D6
rm file500-0
rm file500-1
#D7
rm file250-0
rm file250-1
rm file250-2
rm file250-3
#D8
rm file125-0
rm file125-1
rm file125-2
rm file125-3
rm file125-4
rm file125-5
rm file125-6
rm file125-7
