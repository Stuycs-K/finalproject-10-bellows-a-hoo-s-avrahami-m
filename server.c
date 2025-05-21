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
#include "server.h"
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
    case SIGUSR1:
      {
        recv_user_cmd();
      }
    case SIGUSR2:
      {
        
      }
  }

}

int server_action(int new_socket){
  listening_server_action(new_socket);
}

static int user_read;
int recv_user_cmd(){
  int shellid;
  int msg_ln;
  char cmd[1024] = "";
  read(user_read, &shellid, sizeof(shellid));
  read(user_read, &msg_ln, sizeof(msg_ln));
  read(user_read, cmd, msg_ln);

  printf("RECV: %d, %s\n", shellid, cmd);
}

int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...
    signal(SIGUSR1, sighandler);

    int user_fds[2];
    pipe(user_fds);

    int user_write = user_fds[1];
    
    user_read = user_fds[0];

    int chldid = fork();
    if(chldid==0){
      close(user_read);
      user_console(user_write);
    }
    close(user_write);



    // while(1){
    //   recv_user_cmd();
    // }
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
