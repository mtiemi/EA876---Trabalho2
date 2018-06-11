#!/bin/bash

dir=imgs_test/*

input=`find $dir -maxdepth 0 -name '*.jpg'`

rm output.out
for t in $input
do
  #Para ao levantamento de dados o programa foi rodado 5 vezes para cada entrada
  for i in 0 1 2 3 4
  do
    echo $t > input.in
    #Foi comentada a parte do arquivo que salva a saida em um arquivo de texto
    ./$1 < input.in >> output.out
    ./$2 < input.in >> output.out
    ./$3 < input.in >> output.out
    echo "" >> output.out
  done
done



cat output.out
