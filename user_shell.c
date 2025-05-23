#include "user_console.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv){
  if(argc <=1){
    perror("ERR: you need to put the pid of the server...");
    exit(1);
  }
  int pid;
  
  sscanf(argv[1],"%d", &pid);
  
  user_console(FIFO_NAME, pid);
  return 0;
}