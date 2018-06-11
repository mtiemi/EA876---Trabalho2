
#include <imageprocessing.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define tamanho_blur 10
imagem img;


// acha os indices iniciais para tratar as bordas
int find_index(int index)
{
    int i = 0;

    while(index - i > 0)
    {
        i++;
    }

    return i;
}

// calcula media de um pixel, parametros é  a matriz r/g/b, tamanho do filtro, indices do pixel
float media(float matriz[], int i, int j)
{
    float soma = 0;
    float resultado = 0;
    int contador = 0;
    int aux1 = i + tamanho_blur;
    int aux2 = j + tamanho_blur;
    int aux3 = 0, aux4 = 0;

    if(aux1 > img.width)
    {
        aux1 = i;
    }
    if(aux2 > img.height)
    {
        aux2 = j;
    }

    if(i - tamanho_blur >= 0 && j - tamanho_blur >= 0)
    {
        for(i = i - tamanho_blur; i < aux1; i++)
        {
            for(j = j - tamanho_blur; j < aux2; j++)
            {
                soma = matriz[j*img.width + i] + soma;
                contador++;
            }
        }
        resultado = soma/(contador);

    // caso de borda
    }else if(i - tamanho_blur < 0 && j - tamanho_blur >= 0)
    {
        aux3 = i - find_index(i);
        for(i = aux3; i < aux1; i++)
        {
            for(j = j - tamanho_blur; j < aux2; j++)
            {
                soma = matriz[j*img.width + i] + soma;
                contador++;
            }
        }
        resultado = soma/(contador);

    }else if(i - tamanho_blur >= 0 && j - tamanho_blur < 0)
    {
        aux4 = j - find_index(j);
        for(i = i - tamanho_blur; i < aux1; i++)
        {
            for(j = aux4; j < aux2; j++)
            {
                soma = matriz[j*img.width + i] + soma;
                contador++;
            }
        }
        resultado = soma/(contador);

    }else if(i - tamanho_blur < 0 && j - tamanho_blur < 0)
    {
        aux3 = i - find_index(i);
        aux4 = j - find_index(j);
        for(i = aux3; i < aux1; i++)
        {
            for(j = aux4; j < aux2; j++)
            {
                soma = matriz[j*img.width + i] + soma;
                contador++;
            }
        }
        resultado = soma/(contador);
    }
    if(resultado > 255)resultado = 255;

    return resultado;
}


//blur única linha de execução, parametros sao a matriz r/g/b, a imagem e o tamanho do filtro
void blur(imagem img_out)
{
    for(int i = 0; i < (img.width); i++)
    {
        for(int j = 0; j < (img.height); j++)
        {

            img_out.r[i+ j*img.width] = media(img.r, i, j);
            img_out.b[i+ j*img.width] = media(img.b, i, j);
            img_out.g[i+ j*img.width] = media(img.g, i, j);
        }
    }

}


int main()
{
  clock_t start, end;
  float *r1, *b1, *g1;
  char nome_arquivo[20];
  imagem img_out;

 // scanf("%s", nome_arquivo);
  img = abrir_imagem("imgs_test/cachorro.jpg");
  img_out = abrir_imagem("imgs_test/cachorro.jpg");


  start = clock();

  blur(img_out);

  end = clock();

  //printf("Arquivo da imagem: %s\n", );
  printf("Resolução: %dpixels %dpixels\n", img.width, img.height);
  printf("Tamanho da matriz de convolução:%dx%d\n", tamanho_blur, tamanho_blur);
  printf("Estratégia: Linha de execução única\n");
  printf("Tempo gasto:%f ms\n", 1000*(double)(end - start)/CLOCKS_PER_SEC);
  //printf("%f", 1000*(double)(end - start)/CLOCKS_PER_SEC);

  salvar_imagem("imgs_test/cachorro-out-unica_linha_ex.jpg", &img_out);
  liberar_imagem(&img_out);
  liberar_imagem(&img);

  return 0;
}
