#include <stdio.h>

#define MAX_LEN 1000000

int main(){
  char input[MAX_LEN];
  fread(input, sizeof(char), MAX_LEN, stdin);
  return 0;
}
