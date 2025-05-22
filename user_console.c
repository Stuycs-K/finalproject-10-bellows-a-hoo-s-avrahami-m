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
#include <fcntl.h>
#include <errno.h>
//reap children
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"
#include "networking.h"


void get_input(char * cmd, int * shellid){
  char buffer[1024] = "";
  memset(buffer, 0, sizeof(buffer));
  fgets(buffer, sizeof(buffer), stdin);
  sscanf(buffer, "%d %[^\n]s", shellid, cmd);
}

int user_console(char * fifo_name, int pid){
  int write_end = open(fifo_name, O_WRONLY, 0);
  if(write_end == -1){
    perror("");
    exit(1);
  }
  while(1){
    int shellid;
    char cmd[1024];
    
    printf(">> (d <cmd>):");
    get_input(cmd, &shellid);

    int len_msg = sizeof(char) * (strlen(cmd) + 1);
    //write through the command rpecceded by the ID
    write(write_end, &shellid, sizeof(int));
    write(write_end, &len_msg, sizeof(int));
    write(write_end, cmd, len_msg);

    kill(pid, SIGUSR1);
  }
  return 0;
}
