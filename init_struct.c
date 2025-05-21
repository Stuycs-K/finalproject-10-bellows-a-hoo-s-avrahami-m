#include "init_struct.h"
#include <string.h>
#include <stdio.h>
struct init_struct new_init_struct(char * whoami, char * hostname, char * passwd, char * external_ip){
    struct init_struct init;

    strcpy(init.hostname, hostname);
    strcpy(init.whoami, whoami);
    strcpy(init.passwd, passwd);
    strcpy(init.external_ip, external_ip);

    return init;
}

void print_init_struct(struct init_struct * s){
    printf("=========init struct============\n");
    printf("whoami: %s\n", s->whoami);
    printf("external_ip: %s\n", s->external_ip);
    printf("passwd: %s\n", s->passwd);
    printf("hostname: %s\n", s->hostname);
    printf("===============================\n");
}