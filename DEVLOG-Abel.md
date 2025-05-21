# Dev Log:

This document must be updated daily every time you finish a work session.

## Abel Bellows

### 2025-05-14/13 - initial formation of sudo implantation
I created a c program that mimicks `sudo` to steal passwords and also implants itself as an alias when run.

### 2025-05-15 - research and project reformulation
Stanley and I ideated regarding how to adapt to the project feedback we recieved.

### 2025-05-16 - sudo works
Michael and I worked on testing our sudo dup 2ing

### 2025-05-17 - sudo works
I resolved a problem with the pipe where without writing new lines to the stdin of the sudo it would not every submit the passwords. I also made something that parses the argv -> execvp_ray and then runs it with the intended effect of the sudoer.

### 2025-05-18
Better faking of sudo, reverse shell functionalist, privalege escalation of virus after passwd is stolen
### 2025-05-19
used systems code from old project to create server that will field reverse shell requests.
### 2025-05-20
created the model for the many running programs that communicate for a seamless user attack experience.
created the listening server, user input server, control server model
created listening server talks to uis model
created working init struct to initiate reverse shell connection