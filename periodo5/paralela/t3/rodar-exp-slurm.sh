#!/bin/bash

if [ $# -eq 1 ]; then
    if [ "$1" -eq 1 ]; then
        SBATCH_NODES=1
    elif [ "$1" -eq 2 ]; then
        SBATCH_NODES=2
    elif [ "$1" -eq 3 ]; then
        SBATCH_NODES=4
    else
        echo "exp deve ser 1, 2 ou 3"
        exit 1
    fi
else
    echo "USAGE: ./rodar-exp-slurm.sh <exp>"
    exit 1
fi

# Diretivas SBATCH
#SBATCH --exclusive
#SBATCH --ntasks-per-node=8
#SBATCH --job-name=mpihello_job
#SBATCH --nodes=$SBATCH_NODES

echo -----------------------------------
echo -----------------------------------
echo -----------------------------------
echo ---------- ATENCAO: usar --exclusive na linha de comando
echo -----------------------------------

##### SBATCH --output=saida.out

echo "$0 rodando no host " `hostname`
echo "$0 rodando no host " `hostname` >saida.txt

echo "SLURM_JOB_NAME: " $SLURM_JOB_NAME
echo "SLURM_NODELIST: " $SLURM_NODELIST
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE

if [ ! -f "multi_partition" ]; then
    make
    MAKE_EXECUTED=true
fi

# Executa o programa MPI
mpirun ./multi_partition 8000000

echo "O tempo total dessa shell foi de" $SECONDS "segundos"
echo "SLURM_JOB_NAME: " $SLURM_JOB_NAME
echo "SLURM_NODELIST: " $SLURM_NODELIST
echo "SLURM_JOB_NODELIST: " $SLURM_JOB_NODELIST
echo "SLURM_JOB_CPUS_PER_NODE: " $SLURM_JOB_CPUS_PER_NODE
squeue -j $SLURM_JOBID

if [ "$MAKE_EXECUTED" = true ]; then
    make clean
fi
