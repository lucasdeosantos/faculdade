#!/bin/bash
echo "USAGE: ./gera-planilhas.sh"

NTIMES=5

EXP_A="experiencia_A.csv"
EXP_B="experiencia_B.csv"

if [ ! -f "multi_partition" ]; then
    make
fi

run_experiment() {
    local exp=$1
    local outputFile=$2

    echo "Executando para $exp com 1-8 threads $NTIMES vezes:" > $outputFile
    echo "1 threads,2 threads,3 threads,4 threads,5 threads,6 threads,7 threads,8 threads" >> $outputFile

    for j in $(seq 1 $NTIMES); do
        echo -n "" >> $outputFile
        for i in {1..8}; do
            time=$(./multi_partition $i $exp | grep -oP '(?<=total_time_in_seconds: )[^ ]*')
            echo -n "$time," >> $outputFile
        done
        echo "" >> $outputFile
    done
}

run_experiment "A" $EXP_A
run_experiment "B" $EXP_B

make clean

echo "Experimentos finalizados. Resultados armazenados em $EXP_A e $EXP_B."
