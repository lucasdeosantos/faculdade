#!/bin/bash

# Check if perfSL executable exists, if not, run make
if [ ! -x ./perfSL ]; then
    make
fi

# Execute the program perfSL through LIKWID, and show only DP MFLOP/s
likwid-perfctr -C 1 -g FLOPS_DP -m ./perfSL | grep 'DP' | grep -v 'AVX'
