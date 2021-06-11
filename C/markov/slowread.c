#include <stdio.h>

int main(){
  char input[1000000];
  int i;
  scanf("%c",input + 0);
  for(i = 0; input[i] != 0; ) 
    scanf("%c", input + (++i));

  return 0;
}
