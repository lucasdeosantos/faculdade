#!/bin/bash

# Verifica se o arquivo de entrada foi fornecido
if [ "$#" -ne 1 ]; then
    echo "Uso: $0 <arquivo_de_entrada>"
    exit 1
fi

# Nome do arquivo de entrada
INPUT_FILE=$1

# Verifica se o arquivo de entrada existe
if [ ! -f "$INPUT_FILE" ]; then
    echo "Erro: Arquivo '$INPUT_FILE' não encontrado!"
    exit 1
fi

# Compila o programa
make

# Executa o programa para gerar o problema no formato lp_solve
./modeling < "$INPUT_FILE" > problem.lp

# Elimina os arquivos de compilação 
make clean

# Soluciona o problema usando lp_solve
lp_solve problem.lp > problem_solve.lp

# Exibe o resultado
cat problem_solve.lp

# Remove arquivos gerados
rm *.lp
