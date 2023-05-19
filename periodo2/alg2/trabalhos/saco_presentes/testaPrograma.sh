#!/bin/bash

touch saidaDoPrograma.txt
> saidaDoPrograma.txt

touch saidaDoProgramaFiltrada.txt
> saidaDoProgramaFiltrada.txt

touch saidaEsperada.txt
> saidaEsperada.txt

for file in entradas/*
do
    $1 < $file >> saidaDoPrograma.txt
done

awk 'NR%2==0' saidaDoPrograma.txt > saidaDoProgramaFiltrada.txt

printf "SAIDA DO PROGRAMA:\n"
cat saidaDoProgramaFiltrada.txt

for file in respostas_esperadas/*
do
    cat $file >> saidaEsperada.txt
    echo "" >> saidaEsperada.txt
done 

printf "SAIDA ESPERADA:\n"
cat saidaEsperada.txt
