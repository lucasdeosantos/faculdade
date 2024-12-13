#!/bin/bash

if [ -z "$1" ]; then
    echo "USAGE: ./roda-todos.sh <nElements>"
    exit 1
fi

# Compila os arquivos se parteA ou parteB não existirem
if [ ! -f ./parteA ] || [ ! -f ./parteB ]; then
    echo "Compilando parteA e parteB..."
    make
fi

for i in {1..8}
do
    echo "Executando 10 vezes com $1 elementos e $i threads:"
    for j in {1..10}
    do
        echo "Medição $j:"
        
        # Executa parteA, filtra para mostrar o total time e throughput
        total_time_parteA=$(./parteA $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        throughput_parteA=$(./parteA $1 $i | grep -oP '(?<=Throughput: )[^ ]*')
        echo "parteA Total time: $total_time_parteA seconds"
        echo "parteA Throughput: $throughput_parteA OP/s"

        # Executa parteB, filtra para mostrar o total time e throughput
        total_time_parteB=$(./parteB $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        throughput_parteB=$(./parteB $1 $i | grep -oP '(?<=Throughput: )[^ ]*')
        echo "parteB Total time: $total_time_parteB seconds"
        echo "parteB Throughput: $throughput_parteB OP/s"
    done
done

echo "Limpando os arquivos objeto..."
make clean
