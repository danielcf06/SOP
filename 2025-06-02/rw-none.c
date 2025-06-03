/*
Sistema leitores-escritores, sem coordenação.
Compilar com gcc -Wall rw-none.c -o rw-none -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_READERS 2
#define NUM_WRITERS 3

int shared; // área compartilhada
int num_leitores = 0;
int num_escritores = 0;

pthread_mutex_t mutex;
pthread_mutex_t mutex_escritor;
pthread_cond_t cond_leitor;
pthread_cond_t cond_escritor;

// espera um tempo aleatório entre 0 e n segundos (float)
void espera(int n)
{
  sleep(random() % n);        // pausa entre 0 e n segundos (inteiro)
  usleep(random() % 1000000); // pausa entre 0 e 1 segundo (float)
}

// corpo das tarefas leitoras
void *readerBody(void *id)
{
  long tid = (long)id;

  while (1)
  {
    pthread_mutex_lock(&mutex);
    while (num_escritores > 0)
    {
      pthread_cond_wait(&cond_leitor, &mutex);
    }

    num_leitores++;
    pthread_mutex_unlock(&mutex);

    
    printf("R%02ld: read %d\n", tid, shared);
    espera(2);
    
    pthread_mutex_lock(&mutex);
    printf("R%02ld: out\n", tid);
    num_leitores--;
    espera(2);
    if(num_leitores == 0){
       pthread_cond_signal(&cond_escritor);
    }
    pthread_mutex_unlock(&mutex);
  }
}

// corpo das tarefas escritoras
void *writerBody(void *id)
{
  long tid = (long)id;

  while (1)
  {
    pthread_mutex_lock(&mutex);
    while (num_escritores > 0)
    {
      pthread_cond_wait(&cond_escritor, &mutex);
    }
    num_escritores = 1;
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex_escritor);
    shared = random() % 1000;
    printf("\t\t\tW%02ld: wrote %d\n", tid, shared);
    espera(2);
    pthread_mutex_unlock(&mutex_escritor);
    
    pthread_mutex_lock(&mutex);
    printf("\t\t\tW%02ld: out\n", tid);
    espera(2);
    num_escritores = 0;
    pthread_cond_broadcast(&cond_leitor);
    pthread_cond_signal(&cond_escritor);
    pthread_mutex_unlock(&mutex);
  }
}

int main(int argc, char *argv[])
{
  pthread_t reader[NUM_READERS];
  pthread_t writer[NUM_WRITERS];
  long i;

  shared = 0;
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex_escritor, NULL);  
  pthread_cond_init(&cond_leitor, NULL);
  pthread_cond_init(&cond_escritor, NULL);

  // cria leitores
  for (i = 0; i < NUM_READERS; i++)
    if (pthread_create(&reader[i], NULL, readerBody, (void *)i))
    {
      perror("pthread_create");
      exit(1);
    }

  // cria escritores
  for (i = 0; i < NUM_WRITERS; i++)
    if (pthread_create(&writer[i], NULL, writerBody, (void *)i))
    {
      perror("pthread_create");
      exit(1);
    }

  // main encerra aqui
  pthread_exit(NULL);
}
