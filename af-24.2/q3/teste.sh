#!/bin/bash
gcc  q3.c -g -o q3

casos=4
cont=0

for((i=1; i<=$casos; i++))
do	
	echo '-----------------------------------'
	echo 'testando' caso$i.txt

	./q3 caso$i.txt
	x=$(diff q3_saida.txt resp_caso$i.txt 2>&1)

	echo $x
	if [ "$x" = "" ]; then
		cont=$(expr $cont + 1)
		echo 'Passou' $cont'/'$casos
	else
		echo 'Erro' $cont'/'$casos
	fi
	
done

v=$(valgrind --leak-check=yes ./q3 caso1.txt 2>&1) 

if echo "$v" | grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)"; then
	echo '-----------------------------------'
	echo "Passou nos testes do valgrind."
	echo '-----------------------------------'
else
	echo '-----------------------------------'
	echo "Problemas acusados pelo valgrind..."
	echo '-----------------------------------'
	exit -1
fi
