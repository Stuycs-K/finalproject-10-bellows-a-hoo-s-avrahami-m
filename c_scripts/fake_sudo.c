#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[]) {
  
  printf("pass: ");
  char buffer[100] = "";
  fgets(buffer, sizeof(buffer), stdin);
  if(buffer[strlen(buffer)-1]=='\n'){
    buffer[strlen(buffer)-1]=0;
  }
  if(strcmp("1234", buffer)==0){
    printf("YOUR IN\n");
    return 0;
  }
  printf("YOUR OUT\n");
  return 1;
}
