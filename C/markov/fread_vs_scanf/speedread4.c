#include <stdio.h>

#define MAX_LEN 1000000

int main(){
  char input[MAX_LEN];
  int i, sz;
  sz = fread(input, 4, MAX_LEN / 4, stdin) * 4;
  return 0;
}
