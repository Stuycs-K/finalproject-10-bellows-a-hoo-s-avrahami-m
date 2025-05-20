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

#include "utils.h"
#include "networking.h"
void sighandler(int signo){
  switch(signo){
    case SIGCHLD:
      {
        //reap all available children but don't block
        while(waitpid(-1,NULL,WNOHANG) > 0);
      }
  }

}

int server_action(int new_socket){
  int child = fork();
  if(child != 0){
    printf("sending child ready...\n");
    while(1){
      printf(">>: ");
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
        printf("\n<<: %s\n", response);
        memset(response, 0, sizeof(response));
      }
    }
  }
}
 

int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...

    //set up server listening ...
    int server_fd = setup_server();

    //server loop
    while(1){
        //main server loop
        printf("establishing connection to client...\n");

        int new_socket = accept(server_fd, NULL,NULL); //block until a client tries to connect

        printf("client connected. forking...\n");
        if(fork()==0){//if fork is child
            // do what the server should do
            server_action(new_socket);

            //clean up
            close(new_socket);
            printf("closing connection to client...\n");
            exit(0);
        }

        //if we are not the subserver, close the socket to the client
        close(new_socket);
  }

  return 0;
}
