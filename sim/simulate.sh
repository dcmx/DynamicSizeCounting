#!/bin/bash
cmake --build ./cmake-build-debug --target population_protocols_cpp -- -j 8
./cmake-build-debug/population_protocols_cpp --exp 1 --exp_end 6 --random 1 --adversary false
./cmake-build-debug/population_protocols_cpp --exp 1 --exp_end 6 --random 60 --adversary false
./cmake-build-debug/population_protocols_cpp --exp 1 --exp_end 6 --random 10 --adversary false
./cmake-build-debug/population_protocols_cpp --exp 1 --exp_end 6 --random 1 --adversary true