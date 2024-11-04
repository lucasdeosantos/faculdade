#!/bin/bash
echo "USAGE: ./rodaTodos.sh <nElements>"
 
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
        # Executa partA e filtra para mostrar apenas o total time
        total_time_partA=$(./partA $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        echo "partA Total time: $total_time_partA seconds"

        # Executa partB e filtra para mostrar apenas o total time
        total_time_partB=$(./partB $1 $i | grep -oP '(?<=Total time: )[^ ]*')
        echo "partB Total time: $total_time_partB seconds"
    done
done

echo "Limpando os arquivos de objeto..."
make clean
