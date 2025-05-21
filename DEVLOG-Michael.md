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
