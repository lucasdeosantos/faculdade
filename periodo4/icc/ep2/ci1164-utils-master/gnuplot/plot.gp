#!/usr/bin/gnuplot -c

## set encoding iso_8859_15
set encoding utf

# set terminal qt persist

set grid

set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
# set xrange [0:]
set xlabel  "N (bytes)"

#
# ALTERNATIVA 1: Tabelas em arquivos separados (2 colunas)
#
set key center center # posição da legenda: center, left, right
set datafile separator comma
set ylabel  "<metrica 1>"
set logscale x
set title   "<campo[marcador 1]>"
set terminal qt 0 title "<campo[marcador 1]>"
plot 'plot_exemplo-01.dat' using 1:2 title "<sem otimização>" lc rgb "green" with linespoints, \
     '' using 1:3 title "<com otimização>" lc rgb "blue" with linespoints, \
     'plot_exemplo-02.dat' title "<com avx>" lc rgb "red" with linespoints

pause -1

#
# ALTERNATIVA 2: Tabela com 3 colunas 
#
set key outside right # posição da legenda: outside {left | right}
unset logscale x
set datafile separator whitespace
set ylabel  "<metrica 1>"
set title   "<campo[marcador 1]>"
set terminal qt 1 title "<campo[marcador 1]>"
plot 'plot_exemplo-03.dat' using 1:2 title "<sem otimização>" with linespoints, \
     '' using 1:3 title "<com otimização>" with linespoints

pause -1

#
# ALTERNATIVA 3: Função
#
set style function dots
set ylabel  "<metrica 1>"
set title   "<campo[marcador 1]>"
set terminal qt 2 title "<campo[marcador 1]>"
f(x,y) = cos(x)*cos(y)
splot f(x,y) title "easom" with dots

pause -1

# Gerando figura PNG
set terminal png
set output "funcao__NxMetrica.png"
plot 'plot_exemplo-03.dat' using 1:2 title "<sem otimização>" with linespoints, \
     '' using 1:3 title "<com otimização>" with linespoints
replot
unset output

