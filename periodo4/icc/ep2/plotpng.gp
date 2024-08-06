#!/usr/bin/gnuplot -c
set grid
set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
set xrange [64:2048]
set xlabel  "N (bytes)"

set logscale x

## set datafile separator {whitespace | tab | comma | "<chars>"}
set datafile separator comma


#
# TEMPO
#
ARQ=ARG1."./Resultados/TEMPOS.csv"
set key left top
set logscale y
set ylabel  "Tempo (ms)"
set title   "Tempo"
set terminal pngcairo size 800,600
set output 'Tempo.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output


#
# FLOPS_DP
#
ARQ=ARG1."./Resultados/FLOPS_DP.csv"
set key right top
unset logscale y
set ylabel  "FLOPS DP [MFlops/s]"
set title   "FLOPS DP"
set terminal pngcairo size 800,600
set output 'FLOPS_DP.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output


#
# FLOPS_AVX
#
ARQ=ARG1."./Resultados/FLOPS_AVX.csv"
set key right top
unset logscale y
set ylabel  "FLOPS AVX [MFlops/s]"
set title   "FLOPS AVX"
set terminal pngcairo size 800,600
set output 'FLOPS_AVX.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output


#
# L3
#
ARQ=ARG1."./Resultados/L3.csv"
set key left top
unset logscale y
set ylabel  "L3 [MBytes/s]"
set title   "L3"
set terminal pngcairo size 800,600
set output 'L3.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output


#
# L2CACHE
#
ARQ=ARG1."./Resultados/L2CACHE.csv"
set key right bottom
unset logscale y
set ylabel  "L2 miss ratio"
set title   "L2 miss ratio"
set terminal pngcairo size 800,600
set output 'L2CACHE.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output


#
# ENERGY
#
ARQ=ARG1."./Resultados/ENERGY.csv"
set key center top
unset logscale y
set ylabel  "Energia [J]"
set title   "Energia"
set terminal pngcairo size 800,600
set output 'ENERGY.png'
plot ARQ using 1:2 title "MatVet" lc rgb "green" with linespoints, \
     '' using 1:3 title "MatVet-uj" lc rgb "red" with linespoints, \
     '' using 1:4 title "MatMat" lc rgb "magenta" with linespoints, \
     '' using 1:5 title "MatMat-uj" lc rgb "cyan" with linespoints, \
     '' using 1:6 title "MatMat-b" lc rgb "orange" with linespoints, \
     '' using 1:7 title "MatMat-ujb" lc rgb "black" with linespoints
unset output
