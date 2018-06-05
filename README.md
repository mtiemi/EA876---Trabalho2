# EA876---Trabalho2
O problema com o qual vamos lidar nesse trabalho é a paralelização de filtros de imagem.

Uma imagem é representada, na máquina, por três matrizes. Cada uma delas se refere a um canal (R, G, B) da imagem. Então, r[i][j], g[i][j] e b[i][j] se referem aos valores de R, G e B do pixel (i,j).

Num filtro blur de tamanho N, novas matrizes r', g' e b' são geradas. Para cada uma delas, o pixel (i,j) recebe o valor da média de todos os pixels da matriz original que estão dentro do quadrado que vai de (i-N, j-N) a (i+N, j+N).

O objetivo técnico do trabalho é fazer um programa de computador que permita identificar se a aplicação de um filtro tipo “blur” em uma imagem é mais rápida se ocorrer em uma única linha de execução, em múltiplas threads ou em múltiplos processos.

Para tal, o grupo deverá implementar as três propostas e então avaliar objetivamente seu tempo de execução em imagens pequenas e em imagens grandes.
