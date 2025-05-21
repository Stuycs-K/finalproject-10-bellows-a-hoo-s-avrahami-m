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
  printf("Listening child listening for incoming virus connections...\n");

  int * childFds = (int*)malloc(SIZE * sizeof(int));
  int idCount = 0;

  while(1){
    // This will eventually be some sort of struct
    // Reading the socket
    char connection[1024] = "";
    int bytes;
    while(bytes = read(new_socket, connection, 1024)){
      printf("\n<<CONNECTION>> : %s\n", connection);
      memset(connection, 0, sizeof(connection));
      
      int fds[2];
      pipe(fds);

      int forkResult = fork();
      if (forkResult == -1) err();

      // If parent, add writePipe to childfds, go back to listening for connections
      if (forkResult > 0){
        close(fds[0]);
        childFds[idCount] = fds[1];
	idCount++;
      }

      // If child, call child program with childID, read pipe to read from parent
      else {
        close(fds[1]);
	int childID = idCount;
        interactWithVirus(childID, fds[0]);
        return 0;
      }
    }
  }

  free (childFds);
  return 0;
}

int interactWithVirus(int childID, int readPipe){
  return 0;
}
