
#include <imageprocessing.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define tamanho_blur 10
imagem img;
float *r1, *b1, *g1;

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

void blur_segunda_metade(int n_processo)
{

    for(int i = img.width/2; i < (img.width); i++)
    {
        for(int j = 0; j < (img.height); j++)
        {
			if(n_processo == 0)
			{
				r1[i+ j*img.width] = media(img.r, i, j);
			}
			if(n_processo == 1)
			{
				b1[i+ j*img.width] = media(img.b, i, j);
			}
			if(n_processo == 2)
			{
				g1[i+ j*img.width] = media(img.g, i, j);
			}
        }
    }

}

void blur_primeira_metade(int n_processo)
{
	pid_t filho;

    for(int i = 0; i < (img.width); i++)
    {
        for(int j = 0; j < (img.height); j++)
        {
			if(n_processo == 0)
			{
				r1[i+ j*img.width] = media(img.r, i, j);
				//printf("vetor red i:%d\n", i);
			}
			if(n_processo == 1)
			{
				b1[i+ j*img.width] = media(img.b, i, j);
				//printf("vetor blue i:%d\n", i);
			}
			if(n_processo == 2)
			{
				g1[i+ j*img.width] = media(img.g, i, j);
				//printf("vetor green i:%d\n", i);
			}
        }
    }

    filho = fork();
    if(filho == 0)
    {
    	blur_segunda_metade(n_processo);
    	exit(0);
    }


}


int main()
{
  clock_t start, end;
  char input_nome_arquivo[40];
  char output_nome_arquivo[50] = "imgs_test/results/multiprocess_";
  char *ptr;
  pid_t filho[2];

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  scanf("%s", input_nome_arquivo);
  img = abrir_imagem(input_nome_arquivo);

  //img = abrir_imagem("imgs_test/cachorro.jpg");
  //printf("main_multiprocess: %s\n", nome_arquivo);

  // criando memory map para cada vetor
  r1 = (float*) mmap(NULL, sizeof(float)*img.width*img.height, protection, visibility, 0, 0);
  //if((float) r1 == -1) printf("Erro de alocacao!\n");
  memset(r1, 0, img.width*img.height*sizeof(float));

  b1 = (float*) mmap(NULL, sizeof(float)*img.width*img.height, protection, visibility, 0, 0);
  //if((float) b1 == -1) printf("Erro de alocacao!\n");
  memset(b1, 0, img.width*img.height*sizeof(float));

  g1 = (float*) mmap(NULL, sizeof(float)*img.width*img.height, protection, visibility, 0, 0);
  //if((float) g1 == -1) printf("Erro de alocacao!\n");
  memset(g1, 0, img.width*img.height*sizeof(float));


  //img_out = abrir_imagem("imgs_test/cachorro.jpg");


  start = clock();


  for (int i = 0; i < 3; i++)
  {
  	filho[i] = fork();
  	if(filho[i] == 0)
  	{
  		blur_primeira_metade(i);
  		exit(0);
  	}

  }

   for (int i = 0; i < 3; i++)
  {
    waitpid(filho[i], NULL, 0);
  }

  end = clock();

  for(int i = 0; i < (img.width); i++)
  {
	for(int j = 0; j < (img.height); j++)
    {
    	img.r[i+ j*img.width] = r1[i+ j*img.width];
    	img.b[i+ j*img.width] = b1[i+ j*img.width];
    	img.g[i+ j*img.width] = g1[i+ j*img.width];

    }
  }

  // printf("Arquivo da imagem: %s\n", input_nome_arquivo);
  // printf("Resolução: %dpixels %dpixels\n", img.width, img.height);
  // printf("Tamanho da matriz de convolução:%dx%d\n", tamanho_blur, tamanho_blur);
  // printf("Estratégia: Processos, 6 processos\n");
  // printf("Tempo gasto:%f ms\n", 1000*(double)(end - start)/CLOCKS_PER_SEC);
  // printf("----------------------------------------\n");
  printf(",%f ,[ms]", 1000*(double)(end - start)/CLOCKS_PER_SEC);

  strtok_r(input_nome_arquivo, "/", &ptr);
  strcat(output_nome_arquivo, ptr);
  //printf("\n%s\n", output_nome_arquivo);
  salvar_imagem(output_nome_arquivo, &img);
  //salvar_imagem("imgs_test/results/cachorro-processo.jpg", &img);
  liberar_imagem(&img);


  return 0;
}
