# Dev Log - Michael Avrahami

### 2025-05-15
Copied over code from other repo to this repo. Did some testing with sudo and forked and called execvp with sudo. I'm still unclear as to why sudo isn't waiting for a password

### 2025-05-16
Discussed methods for inputting password to sudo, getting the result from sudo.
Created the pipe which allows the parent to write the password to the child process running sudo.
