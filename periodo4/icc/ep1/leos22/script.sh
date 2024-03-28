#!/bin/bash

likwid-perfctr -C 0-3 -g FLOPS_DP - m ./perfSL 2>/dev/null | grep "DP MFLOP/s"
