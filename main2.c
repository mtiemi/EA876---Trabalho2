
#include <imageprocessing.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define tamanho_blur 10
imagem img;
imagem img_out;

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

void* blur_segunda_metade(void *arg)
{   
	int* N = (int*)arg;
	int n_thread = (*N);
	
    for(int i = img.width/2; i < (img.width); i++) 
    {
        for(int j = 0; j < (img.height); j++) 
        {       		
			if(n_thread == 0)
			{
				img_out.r[i+ j*img.width] = media(img.r, i, j);
			}
			if(n_thread == 1)
			{
				img_out.b[i+ j*img.width] = media(img.b, i, j);
			}
			if(n_thread == 2)
			{
				img_out.g[i+ j*img.width] = media(img.g, i, j);                
			}        
        }
    } 
    return NULL;
}

void* blur_primeira_metade(void *arg)
{   
	int* N = (int*)arg;
	int n_thread = (*N);
	pthread_t threads[n_thread];
  	int thread_args[n_thread];
  	
 	if(n_thread == 0)
	{
		thread_args[n_thread] = n_thread;
		pthread_create(&(threads[n_thread]), NULL, blur_segunda_metade, &(thread_args[n_thread]));
	}
	if(n_thread == 1)
	{
		thread_args[n_thread] = n_thread;
		pthread_create(&(threads[n_thread]), NULL, blur_segunda_metade, &(thread_args[n_thread]));
	}
	if(n_thread == 2)
	{
		thread_args[n_thread] = n_thread;
		pthread_create(&(threads[n_thread]), NULL, blur_segunda_metade, &(thread_args[n_thread]));             
	}   
	
    for(int i = 0; i < (img.width/2); i++) 
    {
        for(int j = 0; j < (img.height); j++) 
        {
       		
			if(n_thread == 0)
			{
				img_out.r[i+ j*img.width] = media(img.r, i, j);
			}
			if(n_thread == 1)
			{
				img_out.b[i+ j*img.width] = media(img.b, i, j);
			}
			if(n_thread == 2)
			{
				img_out.g[i+ j*img.width] = media(img.g, i, j);               
			}        
        }
    } 
    
    
    return NULL;
}


int main() 
{
  clock_t start, end;
  char nome_arquivo[20];
  pthread_t threads[2];
  int thread_args[2];
  
 // scanf("%s", nome_arquivo);
  img = abrir_imagem("imgs_test/cachorro.jpg");
  img_out = abrir_imagem("imgs_test/cachorro.jpg");
  
  img_out.r = (float*)malloc(img.height * img.width * sizeof(float)); 

  img_out.b = (float*)malloc(img.height * img.width * sizeof(float)); 
  
  img_out.g = (float*)malloc(img.height * img.width * sizeof(float)); 
  
  start = clock();

  for (int i = 0; i < 3; i++) 
  {
    thread_args[i] = i;
    pthread_create(&(threads[i]), NULL, blur_primeira_metade, &(thread_args[i]));
  }
  
  /* Esperando threads terminarem! */
  for (int i = 0; i < 3; i++) 
  {
    pthread_join(threads[i], NULL);
  }
  
  end = clock();
  
  //printf("Arquivo da imagem: %s\n", );
  printf("Resolução: %dpixels %dpixels\n", img.width, img.height);
  printf("Tamanho da matriz de convolução:%dx%d\n", tamanho_blur, tamanho_blur);
  printf("Estratégia: threads, 6 threads\n");
  printf("Tempo gasto:%f ms\n", 1000*(double)(end - start)/CLOCKS_PER_SEC); 
  
  salvar_imagem("imgs_test/cachorro-thread.jpg", &img_out);
  liberar_imagem(&img_out);
  
  return 0;
}
