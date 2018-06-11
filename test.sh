#!/bin/bash
dir=imgs_test/*
#echo $dir
input=`find $dir -maxdepth 0 -name '*.jpg'`
cont=0

#echo $input
rm output.out
for t in $input
do
  #echo $t
  echo $t > input.in
  ./$1 < input.in >> output.out
  #cat input.in
done

rm minha_saida.csv
#touch minha_saida.csv
echo "?"
cont=$((cont+1))
echo -n $cont >> minha_saida.csv
#./$1 >> minha_saida.csv
#./$2 >> minha_saida.csv
#./$3 >> minha_saida.csv
echo "" >> minha_saida.csv

echo "=============="
cat minha_saida.csv
