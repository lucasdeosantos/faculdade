#!/bin/bash

if [ -z "$1" ]; then
    echo "USAGE: ./gera-planilha.sh <nElements>"
    exit 1
fi

# Arquivo CSV para armazenar os dados
output_file="resultados.csv"

# Adiciona cabeçalho ao CSV
echo "Thread,Execução,PartA Total Time,PartA Throughput,PartB Total Time,PartB Throughput" > "$output_file"

# Executa o script rodaTodos.sh e captura a saída
while IFS= read -r line; do
    # Verifica se a linha indica a quantidade de threads
    if [[ $line =~ ^Executando\ 10\ vezes\ com ]]; then
        thread_count=$(echo "$line" | grep -oP '(?<=com )[0-9]+(?= threads)')
    fi

    # Verifica se é uma linha de medição
    if [[ $line =~ ^Medição ]]; then
        exec_num=$(echo "$line" | grep -oP '[0-9]+')
    fi

    # Captura os tempos e throughput da partA
    if [[ $line =~ ^partA\ Total\ time ]]; then
        total_time_partA=$(echo "$line" | grep -oP '[0-9.]+(?= seconds)')
    fi
    if [[ $line =~ ^partA\ Throughput ]]; then
        throughput_partA=$(echo "$line" | grep -oP '[0-9.]+(?= OP/s)')
    fi

    # Captura os tempos e throughput da partB
    if [[ $line =~ ^partB\ Total\ time ]]; then
        total_time_partB=$(echo "$line" | grep -oP '[0-9.]+(?= seconds)')
    fi
    if [[ $line =~ ^partB\ Throughput ]]; then
        throughput_partB=$(echo "$line" | grep -oP '[0-9.]+(?= OP/s)')

        # Salva os dados na planilha
        echo "$thread_count,$exec_num,$total_time_partA,$throughput_partA,$total_time_partB,$throughput_partB" >> "$output_file"
    fi
done < <(./roda-todos.sh "$1")

echo "Planilha gerada em $output_file"

