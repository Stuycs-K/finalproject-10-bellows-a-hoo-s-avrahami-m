#include "user_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

//signal stuff
#include <signal.h>

//reap children
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"
#include "networking.h"

void get_input(char * cmd, int * shellid){
  char buffer[1024] = "";
  memset(buffer, 0, sizeof(buffer));
  printf(">> (d <cmd>):");
  fgets(buffer, sizeof(buffer), stdin);

  sscanf(buffer, "%d %s", shellid, cmd);
}
int user_console(int write_end){


  while(1){
    int shellid;
    char cmd[1024];

    get_input(cmd, &shellid);

    //write through the command rpecceded by the ID
    write(write_end, &shellid, sizeof(int));
    write(write_end, cmd, sizeof(char) * (strlen(cmd) + 1));
  }
  return 0;
}
