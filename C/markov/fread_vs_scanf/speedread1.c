#include <stdio.h>

#define MAX_LEN 1000000

int main(){
  char input[MAX_LEN];
  int i, sz;
  sz = fread(input, 1, MAX_LEN, stdin);
  return 0;
}
