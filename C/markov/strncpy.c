#include <stdio.h>
#include <string.h>

int main(){
  int n = 3;
  char str1[] = "victor moraes";
  char str2[] = "sei la";
  char str3[] = "victor moraes";
  char str4[] = "sei la";
  strcpy(str3,str4);
  printf("%s\n", str3);
  strncpy(str1,str2,n);
  printf("%s\n", str1);

  return 0;
}

