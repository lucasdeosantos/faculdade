#!/bin/bash

# Forma de uso:
#
#         perfctr <CORE_ID> <GRUPO_PERFORMANCE> ./matmult <opcoe_matmult>
#
# Exemplo, para fazer as medições de performance de FLOPS_DP no core 3
#
#         perfctr 3 FLOPS_DP ./matmult 64
#
CPU=3
LIKWID_CMD="likwid-perfctr -C ${CPU} -g $1 -m"
shift 1

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make purge matmult

${LIKWID_CMD} $*

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

# Para obter topologia dos cpu's
#      likwid-topology -c -g

# Para obter lista de grupos de indicadores de performance:
#      likwid-perfctr -a

# Para obter lista de Eventos e Contadores
#      likwid-perfctr -e