#!/bin/bash

if [ $# -ne 1 ]; then
    echo "USAGE: ./roda-todos-slurm.sh <exp>"
    exit 1
fi

EXP=$1

if [[ "$EXP" != "A" && "$EXP" != "B" ]]; then
    echo "Erro: <exp> deve ser 'A' ou 'B'."
    exit 1
fi

echo "$0 rodando no host " `hostname`
echo "$0 rodando no host " `hostname` > saida.txt

echo "SLURM_JOB_NAME: "  $SLURM_JOB_NAME
echo "SLURM_NODELIST: " $SLURM_NODELIST
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

NTIMES=10
MAX_THREADS=8
MAX_EXECS=5

echo "NTIMES = $NTIMES"
echo "MAX_THREADS = $MAX_THREADS"
echo "EXP = $EXP"

if [ ! -f "multi_partition" ]; then
    make
    MAKE_EXECUTED=true
fi

for i in $(seq 1 $MAX_THREADS); do
    echo "Executando $NTIMES vezes com configuração EXP=$EXP e $i threads:"
    for j in $(seq 1 $NTIMES); do
        echo "-----------------------" >>saida.txt
        if [ $j -le $MAX_EXECS ]; then
            ./multi_partition $i $EXP | tee -a saida.txt | grep -oP '(?<=total_time_in_seconds: )[^ ]*'
        else
            echo "nao executado" | tee -a saida.txt
        fi
    done
done

echo "O tempo total dessa shell foi de $SECONDS segundos"
echo "SLURM_JOB_NAME: " $SLURM_JOB_NAME
echo "SLURM_NODELIST: " $SLURM_NODELIST
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

squeue -j $SLURM_JOBID

if [ "$MAKE_EXECUTED" = true ]; then
    make clean
fi
