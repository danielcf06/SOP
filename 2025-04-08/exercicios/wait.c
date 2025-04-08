#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (){
    pid_t pidAux;
    int status, valor = 5, fatorial = 0;
    int resposta = 1;

    while(valor > 1){
        pidAux = fork();

        if(pidAux == 0){
            resposta *= valor;
            return resposta;
        }else{
            wait(&status);
            if(WIFEXITED(status)){
                resposta = WEXITSTATUS(status);
                valor--;
            }
        }
    }

    printf("%d\n", fatorial);
    return 0;

    
}