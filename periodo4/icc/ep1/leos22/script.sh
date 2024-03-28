#!/bin/bash

# Execute the program perfSL through LIKWID
likwid-perfctr -C 0-3 -g FLOPS_DP ./perfSL 2>/dev/null | grep "|      DP MFLOP/s      |"
