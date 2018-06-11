#!/bin/bash
dir=imgs_test/*
#echo $dir
input=`find $dir -maxdepth 0 -name '*.jpg'`
cont=0
max=0

#echo $input
rm output.out
for t in $input
do
  #echo $t
  #for i in `seq 2 $max`
  for i in 0 1 2 3 4 5 6 7 8 9
  do
    echo $t > input.in
    ./$1 < input.in >> output.out
    ./$2 < input.in >> output.out
    ./$3 < input.in >> output.out
    echo "" >> output.out
  done
  #cat input.in
done

# rm minha_saida.csv
# #touch minha_saida.csv
# echo "?"
# cont=$((cont+1))
# echo -n $cont >> minha_saida.csv
# #./$1 >> minha_saida.csv
# #./$2 >> minha_saida.csv
# #./$3 >> minha_saida.csv
# echo "" >> minha_saida.csv

echo "=============="
# cat minha_saida.csv
cat output.out
