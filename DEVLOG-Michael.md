# Dev Log - Michael Avrahami

### 2025-05-15
Copied over code from other repo to this repo. Did some testing with sudo and forked and called execvp with sudo. I'm still unclear as to why sudo isn't waiting for a password

### 2025-05-16
Discussed methods for inputting password to sudo, getting the result from sudo.
Created the pipe which allows the parent to write the password to the child process running sudo.

### 2025-05-19
Discussed how to create a server that would receive a reverse shell from sudo. Began implementing the reverse shell by creating the server that would listen for connections.

### 2025-05-20
Implemented the listening server by listening to incoming connections on the socket. Any time a connection occurs, the listening server forks to get the child to deal with the incoming connection, and goes back to listening.

### 2025-05-21
Wrote some clean up code to make the sudo side of the code more manageable. Prepared for working more functionality into root mode of sudo.

### 2025-05-22
Worked on implementing the listener server and user console by crating the pipe between the user consol and listening server such that the listener would receive instructions from the user

### 2025-05-23
Created functionality for root mode in sudo by having pseudo create the reverse shell which the listener would then receive and be able to work as if they were on the target machine.

### 2025-05-27
Added working directory to init struct so user knows the directory of the virus. Also began work on destroy mode of pseudo.

### 2025-05-28
Worked on making the project a little easier to navigate, cleaned up makefile and extraneous print statements

### 2025-05-29
Discovered a bug in the program that was due to extra servers not being closed properly upon virus termination. Looked into fixing it via pipes

### 2025-06-02
Cleaned up some of the printing extra commands. Also worked on trying to use a pipe to close the child server who isn't aware when a virus exits.
