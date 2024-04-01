#!/bin/bash

# Check if perfSL executable exists, if not, run make
if [ ! -x ./perfSL ]; then
    make
fi

# Execute the program perfSL through LIKWID, and show only DP MFLOP/s
likwid-perfctr -C 0-3 -g FLOPS_DP ./perfSL 2>/dev/null 
# | grep "|      DP MFLOP/s      |"
