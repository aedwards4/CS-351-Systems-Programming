#!/bin/bash

prec=(single double)
size=(1024 4096 16384)
size2=(10 100 1000)
threads=(1 2 4)

#flops
for x in ${prec[@]}; do

  for y in ${size2[@]}; do

    for z in ${threads[@]}; do

      ./cpubench flops ${x} ${y} ${z}

    done

  done

done


#matrix multiplication
for x in ${prec[@]}; do

  for y in ${size[@]}; do

    for z in ${threads[@]}; do

      ./cpubench matrix ${x} ${y} ${z}

    done

  done

done
