#!/bin/bash

#set -x
 
axml=$t1 # arquivo de entrada no formato xml
acsv=$t2 # arquivo de saida no formato csv

if [ "$#" -eq 2 ]
then
	/home/soft/xgrep/bin/xgrep -t -x "//PMID|//AMID|//ArticleTitle|//Abstract|//MeshHeadingList" $axml
else
	echo 'numero incorreto de parametros'
	echo 'uso: <axml> <acsv>'
	exit 1
fi
