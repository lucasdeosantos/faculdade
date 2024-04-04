#!/bin/bash

# Check if perfSL executable exists, if not, run make
if [ ! -x ./perfSL ]; then
    make
fi

# Export the likwid library path
export LD_LIBRARY_PATH=/home/soft/likwid/lib:$(LD_LIBRARY_PATH)

# Execute the program perfSL through LIKWID, and show only DP MFLOP/s
likwid-perfctr -C 8 -g FLOPS_DP -m ./perfSL | grep 'DP' | grep -v 'AVX'
