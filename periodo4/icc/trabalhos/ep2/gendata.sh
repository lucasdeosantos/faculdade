#!/bin/bash

PROG=matmult
CPU=3

export DATA_DIR="Resultados"

mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make purge likwid

METRICA="FLOPS_DP L2CACHE L3"
TEMPOS="${DATA_DIR}/Tempos.csv"
TAMANHOS="64 100 128 200 256 512 600 900 1024 2000 2048 4000"

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}.csv"
    rm -f ${LIKWID_LOG}
    rm -f ${TEMPOS}
    
    for n in $TAMANHOS
    do
	LIKWID_OUT="${DATA_DIR}/${m}_${n}.txt"
	rm -f ${LIKWID_OUT}
	
	echo "--->>  $m: ./${PROG} $n" >/dev/tty
	likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} ${n} >>${TEMPOS}

	# A partir dos LOGS, gera arquivos CSV com 3 colunas: N, metrica_matvet, metrica_matmat
	# ./genplot.sh ${m} ${n} >> ${LIKWID_LOG}
	./genplot.py ${m} ${n} ${DATA_DIR} >> ${LIKWID_LOG}
    done

done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

