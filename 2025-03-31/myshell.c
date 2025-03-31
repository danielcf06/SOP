#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/stat.h>


int mycwd() {
  // char *getcwd(char buf[.size], size_t size);
  char buff[PATH_MAX];
  if(getcwd(buff, sizeof(buff)) != NULL){
    printf("%s\n", buff);
    return 0;
  } else {
    printf("Algo não deu certo!\n");
    return 1;
  }
  
  
}

int mymkdir() {
  // int mkdir(const char *pathname, mode_t mode);
  char path[PATH_MAX];
  scanf("%s", path);
  if(mkdir(path, 0777) == 0){
    printf("Diretório %s, criado!\n", path);
    return 0;
  }else{
    printf("Algo não deu certo!\n");
    return 1;
  }

}

int myrmdir() {
  // int rmdir(const char *pathname);
  char path[PATH_MAX];
  scanf("%s",path);
  if(rmdir(path) == 0){
    printf("Diretório %s removido\n", path);
    return 0;
  } else {
    printf("Diretório %s não encontrado!\n");
    return 1;
  }

}

int mycd() {
  // int chdir(const char *path);
  char path[PATH_MAX];
  scanf("%s", path);
  if(chdir(path) == 0){
    printf("Você está no diretório %s\n", path);
    return 0;
  } else {
    printf("Diretório %s não encontrado!\n");
    return 1;
  }
}

int mystat() {
  // int stat(const char *restrict pathname, struct stat *restrict statbuf);
  struct stat sb;
  char path[PATH_MAX];
  scanf("%s", &path);
  if(stat(path, &sb) == 0){
      printf("Tamanho: %lld bytes\n", (long) sb.st_size);
      printf("Ligações: %d\n", sb.st_nlink);
      return 0;
  } else {
    return 1;
  }


}

int myls() {
  // DIR *opendir(const char *name);
  // struct dirent *readdir(DIR *dirp);
  // int closedir(DIR *dirp);
  DIR *dir;
  struct dirent *dp;
  char path[PATH_MAX];
  char name[1025];

  if((dir = opendir (".") == NULL) || (dir = opendir(path) == NULL)){
    printf("Algo deu errado!\n");
    return 1;
  }else{
    while ((dp = readdir(dir)) != NULL)
    {
      if(dp->d_name[0] != '.'){
        strcpy(name, path);
        strcat(name, "/");
        strcat(name, dp->d_name);
        printf(name);
    }
    closedir(dir);
      }
    }
  return 0; 
}


int main(int argc, char** argv) {
  int test = 0;
  while(test == 0) {
    char in[60];
    printf("myshell> ");
    scanf("%s", in);
  
    if(strcmp(in, "exit") == 0) {
      test = 1;
    } else if(strcmp(in, "cwd") == 0) {
      mycwd();
    } else if(strcmp(in, "mkdir") == 0) {
      mymkdir();
    } else if(strcmp(in, "rmdir") == 0) {
      myrmdir();
    } else if(strcmp(in, "cd") == 0) {
      mycd();
    } else if(strcmp(in, "stat") == 0) {
      mystat();
    } else if(strcmp(in, "ls") == 0) {
      myls();
    } else {
      printf("Comando não encontrado\n");
    }
  }
}
