// Arquivo mq-send.c: envia mensagens para uma fila de mensagens Posix.
// Em Linux, compile usando: cc -Wall mqsend.c -o mqsend -lrt
// Carlos Maziero, DINF/UFPR, Julho de 2020

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE "/my_queue"

int main (int argc, char *argv[])
{
  mqd_t  queue ;         // descritor da fila
  int    msg ;           // mensagem a enviar

  // abre a fila de mensagens, se existir
  if((queue = mq_open (QUEUE, O_RDWR)) < 0)
  {
    perror ("mq_open") ;
    exit (1) ;
  }

  for (int i = 0; i <= 10; i++)
  {
    msg = random() % 100 ;  // valor entre 0 e 99

    if(i != 10){
      if (mq_send (queue, (void*) &msg, sizeof(msg), 0) < 0)
      {
        perror ("mq_send") ;
        exit (1) ;
      }
    }else{
      msg = -1;
      if (mq_send (queue, (void*) &msg, sizeof(msg), 0) < 0)
      {
        perror ("mq_send") ;
        exit (1) ;
      }
    } 
    sleep (1) ;
  }
}
