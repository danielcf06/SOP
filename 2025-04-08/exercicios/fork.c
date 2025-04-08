#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (){
    pid_t pidA, pidB, pidC;
    int status;
    
    pidA = getpid();
    pidB = fork();
    
    if(pidB == 0){
        pid_t bAUX = pidB;
        pidC = fork();
        if(pidC == 0){
            printf("Eu sou o processo %d, filho de %d\n", getpid(), getppid());
            sleep(1);
        }else{
            wait(NULL);
            printf("Eu sou o processo %d, filho de %d\n", getpid(), getppid());
            sleep(1);
        }
    }else{
        wait(NULL);
        printf("Eu sou o processo %d, filho de %d\n", getpid(), getppid());
    }
    exit(0);
    
}