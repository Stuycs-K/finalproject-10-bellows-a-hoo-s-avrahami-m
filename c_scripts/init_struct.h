#ifndef INIT_STRUCT_H
#define INIT_STRUCT_H

struct init_struct {
    char whoami[256];
    char hostname[256];
    char passwd[1024];
    char external_ip[256];
    char curr_directory[1024];
};

struct init_struct new_init_struct(char * whoami, char * hostname, char * passwd, char * external_ip, char * curr_directory);
void print_init_struct(struct init_struct * s);
#endif
