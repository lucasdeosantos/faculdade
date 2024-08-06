#!/bin/bash

PROG=matmult
CPU=3

make purge matmult

export DATA_DIR="Resultados"
mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

METRICA="FLOPS_DP L2CACHE L3 ENERGY"
TEMPOS="${DATA_DIR}/TEMPOS.csv"
TAMANHOS="64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000 5000 6000 10000"

FLOPS_AVX_LOG="${DATA_DIR}/FLOPS_AVX.csv"
rm -f ${FLOPS_AVX_LOG}

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}.csv"
    rm -f ${LIKWID_LOG}
    rm -f ${TEMPOS}

    for n in $TAMANHOS
    do
        LIKWID_OUT="${DATA_DIR}/${m}_${n}.txt"
        rm -f ${LIKWID_OUT}

        echo "--->> ${m}: ./${PROG} $n" >/dev/tty
        likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} ${n} >>${TEMPOS}

        if [ "$m" = "FLOPS_DP" ]; then
            cp ${LIKWID_OUT} "${DATA_DIR}/FLOPS_AVX_${n}.txt"
            ./genplot.py "FLOPS_AVX" ${n} ${DATA_DIR} >> ${FLOPS_AVX_LOG}
        fi

        # Gera arquivos CSV a partir dos logs
        ./genplot.py ${m} ${n} ${DATA_DIR} >> ${LIKWID_LOG}
    done
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor
