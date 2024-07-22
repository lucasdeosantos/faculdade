#! /usr/bin/env python3
# coding=utf8

# Descomente as 2 linhas abaixo se quiser que Python procure por modulos
# em outros diretórios que não os do sistema e o diretório corrente.
## import sys, os
## sys.path.extend([ '~/lib/python', '.', '..' ])
# Ou em seu ambiente shell, crie e exporte a variavel de ambiente PYTHONPATH.
# Coloque a linha abaixo em seu arquivo '~/.bashrc' ou '~/.profile:
## export PYTHONPATH="${HOME}/lib/python:.:.."
#

# import numpy as np
# import matplotlib.pyplot as plt

from string import *
from math import *
import re, sys, os


#  "CACHE" : "data cache miss ratio",
#  "CACHE" : "data cache miss rate",
#  "L2CACHE" : "L2 miss rate",
#  "MEM" : "Memory read bandwidth",
#  "MEM" : "Memory write bandwidth",
#  "TIME" : "Runtime \(RDTSC\) \[*s\]*"

campos = { "CACHE" : "data cache misses",
           "L2CACHE" : "L2 miss ratio",
           "L3" : "L3 bandwidth \[*MBytes/s\]*",
           "MEM" : "Memory bandwidth \[*MBytes/s\]*",
           "FLOPS_DP" : "DP \[*MFLOP/s\]*",
           "FLOPS_AVX" : "AVX DP \[*MFLOP/s\]*",
           "ENERGY" : "Energy \[*J\]*"
         }
    

# =====================================================
# Inicio Programa principal
# =====================================================
#
# Descomente a linha abaixo se arquivo pode ser importado como módulo
# via 'import .....'
## if __name__ == '__main__':

# Lê saída de likwid-perfctr -O e gera saída para gnuplot gerar
# um gráfico para cada métrica, cada gráfico contendo os dados de todos os markers
# 

metrica=sys.argv[1]
tam=sys.argv[2]
dados=sys.argv[3]+"/"+sys.argv[1]+"_"+sys.argv[2]+".txt"

# print(tam+',', end='')
n=tam+','

os.system("echo -n "+n+";cat "+dados+" | egrep '^"+campos[metrica]+"' | cut -d',' -f 2 |  awk '{print}' ORS=',' | sed -e 's;,$;;g'")

print()
