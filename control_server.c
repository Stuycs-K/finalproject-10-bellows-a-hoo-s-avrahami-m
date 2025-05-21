#include "control_server.h"
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
#include "server.h"

#define SIZE 100

int listening_server_action(int new_socket){
   while (1){
    //printf("Server waiting for a socket connection...\n");

    int child = fork();
    if(child != 0){
      printf("Wa...\n");
      while(1){
        printf("%d >>: ", getpid());
        char cmd[1024] = "";
        fgets(cmd, sizeof(cmd), stdin);
        write(new_socket, cmd, (strlen(cmd)+1) * sizeof(char));
        printf("\n");
      }
    }
    else{
      printf("Listening child ready...\n");
      while(1){
        char response[1024] = "";
        int bytes;
        while(bytes = read(new_socket, response, 1024)){
          printf("\n<<RESPONSE>> : %s\n", response);
          memset(response, 0, sizeof(response));
        }
        printf("%d >>: ", getpid());
      }
    }
  }
}
