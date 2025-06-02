#ifndef CONTROL_SERVER_H
#define CONTROL_SERVER_H

int listening_server_action(int new_socket, int readPipe);
extern char * server_name;
#define LISTENING_SERVER "listening server"
#define CONTROL_SERVER_WRITE "control server that writes to shell"
#define CONTROL_SERVER_READ "control server that reads from shell"
#endif
