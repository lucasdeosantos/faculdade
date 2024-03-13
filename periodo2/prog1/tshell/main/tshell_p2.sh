#!/bin/bash

# set -x

dcsv=$1 # diretorio que contem os arquivos csv
dsin=$2 # diretorio que contem as tabelas de sinonimos

# testa se o numero de parametros é igual a 2
if [ "$#" -eq 2 ]
then

	# acessa o diretorio onde estao as tabelas de sinonimos
	cd $dsin

	# passa por todas as tabelas de sinonimos
	for sinonimo in *.txt
	do
		i=0 # i é um contador
		
		# passa por todos os arquivos csv
		for file in $dcsv/*.csv
		do
			# incrementa o contador com o resultado do grep
			let i=$(grep -w -F -c -f $sinonimo $file)+$i
		done
		
		# imprime o resultado
		echo "${sinonimo%.txt}:$i"
	done

else
	echo 'numero incorreto de parametros'
	echo 'uso: <dcsv> <dsin>'
	exit 1
fi
