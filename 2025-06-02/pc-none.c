/*
Sistema produtor-consumidor sem coordenação.
Compilar com gcc -Wall pc-none.c -o pc-none -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PROD 5
#define NUM_CONS 3
#define VAGAS 6

int num_itens, num_vagas ; 	// contadores de vagas e peças
pthread_mutex_t mutex_vagas; 
pthread_mutex_t mutex_num_itens; 
pthread_mutex_t mutex_buffer; 
pthread_cond_t cond_produtor; 
pthread_cond_t cond_consumidor; 

// espera um tempo aleatório entre 0 e n segundos (float)
void espera (int n)
{
  sleep (random() % n) ;	// pausa entre 0 e n segundos (inteiro)
  usleep (random() % 1000000) ;	// pausa entre 0 e 1 segundo (float)
}

// corpo de produtor
void *prodBody (void *id)
{
  long tid = (long) id ;
  printf ("P%02ld: Olá!\n", tid) ;

  while (1)
  {  
    pthread_mutex_lock(&mutex_vagas);
    while(num_vagas == 0){
      pthread_cond_wait(&cond_produtor, &mutex_vagas);
    }
    num_vagas-- ;
    pthread_mutex_unlock(&mutex_vagas);

    pthread_mutex_lock(&mutex_num_itens);
    num_itens++ ;
    pthread_cond_signal(&cond_consumidor);
    pthread_mutex_unlock(&mutex_num_itens);

    pthread_mutex_lock(&mutex_buffer);
    printf ("P%02ld: put an item (%d itens, %d vagas)!\n",
            tid, num_itens, num_vagas) ;
    pthread_mutex_unlock(&mutex_buffer);

    
    espera (2) ;
  }
}

// corpo de consumidor
void *consBody (void *id)
{
  long tid = (long) id ;
  printf ("C%02ld: Olá!\n", tid) ;

  while (1)
  {
    pthread_mutex_lock(&mutex_num_itens);
    while(num_itens == 0){
      pthread_cond_wait(&cond_consumidor, &mutex_num_itens);
    }
    num_itens-- ;
    pthread_mutex_unlock(&mutex_num_itens);

    pthread_mutex_lock(&mutex_vagas);
    num_vagas++ ;
    pthread_cond_signal(&cond_produtor);
    pthread_mutex_unlock(&mutex_vagas);

    pthread_mutex_lock(&mutex_buffer);
    printf ("C%02ld: got an item (%d itens, %d vagas)!\n",
            tid, num_itens, num_vagas) ;
    pthread_mutex_unlock(&mutex_buffer);


    espera (2) ;
  }
}

// programa principal
int main (int argc, char *argv[])
{
  pthread_t produtor   [NUM_PROD] ;
  pthread_t consumidor [NUM_CONS] ;
  long i ;

  num_itens = 0 ;
  num_vagas = VAGAS ;

  pthread_mutex_init(&mutex_vagas, NULL);
  pthread_mutex_init(&mutex_buffer, NULL);
  pthread_mutex_init(&mutex_num_itens, NULL);
  pthread_cond_init(&cond_consumidor, NULL);
  pthread_cond_init(&cond_produtor, NULL);

  // cria produtores
  for (i=0; i<NUM_PROD; i++)
    if (pthread_create (&produtor[i], NULL, prodBody, (void *) i))
    {
      perror ("pthread_create") ;
      exit (1) ;
    }

  // cria consumidores
  for (i=0; i<NUM_CONS; i++)
    if (pthread_create (&consumidor[i], NULL, consBody, (void *) i))
    {
      perror ("pthread_create") ;
      exit (1) ;
    }

  pthread_exit (NULL) ;
}
