#!/bin/bash
echo "USAGE: ./rodaTodos.sh <nElements>"

# Compila os arquivos se partA ou partB não existirem
if [ ! -f ./partA ] || [ ! -f ./partB ]; then
    echo "Compilando partA e partB..."
    make
fi

for i in {1..8}
do
    echo "Executando 10 vezes com $1 elementos e $i threads:"
    for j in {1..10}
    do
        echo "Medição $j:"
        
        # Executa partA, filtra para mostrar o total time e throughput
        total_time_partA=$(./partA $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        throughput_partA=$(./partA $1 $i | grep -oP '(?<=Throughput: )[^ ]*')
        echo "partA Total time: $total_time_partA seconds"
        echo "partA Throughput: $throughput_partA OP/s"

        # Executa partB, filtra para mostrar o total time e throughput
        total_time_partB=$(./partB $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        throughput_partB=$(./partB $1 $i | grep -oP '(?<=Throughput: )[^ ]*')
        echo "partB Total time: $total_time_partB seconds"
        echo "partB Throughput: $throughput_partB OP/s"
    done
done

echo "Limpando os arquivos objeto..."
make clean
