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
#include "control_server.h"
#include "user_console.h"

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
  listening_server_action(new_socket);
}

int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...

    int chldid = fork();
    if(chldid==0){
      user_console();
    }

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
