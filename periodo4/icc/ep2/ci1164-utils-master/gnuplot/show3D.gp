#!/usr/bin/gnuplot -c
## set encoding iso_8859_15
set encoding utf
## set terminal qt persist
set grid
set style data point
set style function dots
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
set ytics
set ztics

#
# EASOM
#
set xlabel  "X"
set ylabel  "Y"
set zlabel  "Z"
set title   "EASOM"
set terminal qt 0 title "EASOM"

set xrange [0:6]; set yrange [0:6]
set style function dots
set isosamples 150,150;

f(x,y) = cos(x) * cos(y) * exp(-(x - pi)**2 - (y - pi)**2)

splot f(x,y)

#
# Styblinski-Tang
#
set xlabel  "X"
set ylabel  "Y"
set zlabel  "Z"
set title   "Styblinski-Tang"
set terminal qt 1 title "Styblinski-Tang"

set xrange [-4:4]; set yrange [-4:4];
set style function dots
set isosamples 150,150;

f(x,y) = 80 + (x**4 - 16*(x**2) + 5*x + y**4 - 16*(y**2) + 5*y)/2

splot f(x,y)

#
# Função qualquer
#
set xlabel  "X"
set ylabel  "Y"
set zlabel  "Z"
set title   "Função qualquer"
set terminal qt 2 title "Função qualquer"

set xrange [-10000:10000]; set yrange [-10000:10000];
set style function dots
set isosamples 150,150;

f(x,y) = 10e5 * x**2 + y**2 - (x**2 + y**2)**2 + 10e-5 * (x**2 + y**2)**4

splot f(x,y)

pause -1

