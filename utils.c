
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// verbose err checking function
// checks if sigerr is equal to -1 (err code)
// if yes it perrors the message (which also echecks errno)
// then it exits if _exit is set to true.
int v_err(int sigerr, char *msg, int _exit) {
    if (sigerr < 0) {
        perror(msg);
        if (_exit) {
            exit(1);
        }
    }
    return sigerr;
}

// less verbose version of v_err for easier implementation
void err() {
    printf("ERROR %d: %s\n", errno, strerror(errno));
    exit(errno);
}
