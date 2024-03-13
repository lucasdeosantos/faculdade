#!/bin/bash

# set -x

dpm=$1 # diretorio que contem os arquivos csv de entrada
ds=$2 #	diretorio onde serao escritos os arquivos de saida
t1=$3 # primeiro parametro de busca
t2=$4 # segundo parametro de busca
t3=$5 # terceiro parametro de busca

# testa se o numero de parametros é maior que 2 e menor ou igual a 5
if [ "$#" -gt 2 -a "$#" -le 5 ]
then

	# acessa o diretorio onde estao os arquivos de entrada
	cd $dpm
	
	# para 1 parametro de busca
	if [ "$#" -eq 3 ]
	then
		for file in *.csv
		do
			grep $t1 $file > $ds/$file
		done 
	
	# para 2 parametros de busca
	elif [ "$#" -eq 4 ]
	then
		for file in *.csv
		do
			grep $t1 $file | grep $t2 > $ds/$file
		done 

	# para 3 parametros de busca 
	else 
		for file in *.csv
		do
			grep $t1 $file | grep $t2 | grep $t3 > $ds/$file
		done 
	fi
else
	echo 'numero incorreto de parâmetros'
	echo 'uso: <dpm> <ds> <t1> [<t2> [<t3> ]]'
	exit 1
fi
