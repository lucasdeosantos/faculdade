#!/bin/bash

# Check if perfSL executable exists, if not, run make
if [ ! -x ./perfSL ]; then
    make > /dev/null
fi

# Export the likwid library path
export LD_LIBRARY_PATH=/home/soft/likwid/lib:$LD_LIBRARY_PATH

# Get number of availables CPU cores
NUM_CORES=$(lscpu | grep '^CPU(s):' | awk '{print $2}')

# Execute the program perfSL through LIKWID, and show only DP MFLOP/s
likwid-perfctr -C 0-$((NUM_CORES - 1)) -g FLOPS_DP -m ./perfSL | grep 'DP' | grep -v 'AVX'
