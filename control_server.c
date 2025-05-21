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


int listening_server_action(int new_socket, int readPipe){
  printf("Listening child listening for incoming virus connections...\n");

  int fds[2];
  pipe(fds);

  int forkResult = fork();

// If parent, read from listening server and write to socket
  if (forkResult == 0){
    while (1){
      int messageLength = 2048;
      char * message = malloc(messageLength * sizeof(char));
      int readResult = read(readPipe, message, messageLength);
      printf("message: %s\n", message);
    }
  }
  // If child, just read from socket
  else {
    while(1){
      char connection[1024] = "";
      int bytes;
      while(bytes = read(new_socket, connection, 1024)){
        printf("\n<<CONNECTION>> : %s\n", connection);
        memset(connection, 0, sizeof(connection));
      }
    }
  }
  return 0;
}
