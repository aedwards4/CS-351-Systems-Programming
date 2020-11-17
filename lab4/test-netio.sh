#!/bin/bash

ops=(add subtract multiply divide)

#function call
for x in ${ops[@]}; do

  ./netio function ${x} 1000000000 C

done

#pipe call
for x in ${ops[@]}; do

  mkfifo /tmp/pipe
  mkfifo /tmp/pipe02

  ./netio pipe ${x} 1000000 S &

  ./netio pipe ${x} 1000000 C

done

#socket call
for x in ${ops[@]}; do

  ./netio socket ${x} 1000000 C &
  ./netio socket ${x} 1000000 S

done

#RPCGEN - Uncomment below
#for x in ${ops[@]}; do

#  ./netio rpc ${x} 1000000000 C

#done
