#include "argparser.h"

long mystrtol(char * str, int base){
  long li;
  char * endptr;
  if(str == NULL){
    fprintf(stderr, "(mystrtol) Passed null pointer as string.\n");
    exit(STRTOL_ERROR);
  }

  li = strtol(str, &endptr, base);

  if(endptr == str){
    fprintf(stderr, "(mystrtol) String is empty.\n");
    exit(STRTOL_ERROR);
  }

  if(*endptr != '\0'){
    fprintf(stderr, "(mystrtol) Failed to convert str to long.\n");
    fprintf(stderr, "First failed character: <%c - 0x%x>.\n", *endptr, *endptr);
    exit(STRTOL_ERROR);
  }

  /* In case the inputed value is not within the ranges of LONG,
   * it just returns LONG_MIN or LONG_MAX. 
   * Might not be desired. Fixing it for every base is not reasonable. 
   * Let's hope people are so kind as to not enter something greater
   * (or smaller) than 9223372036854775807 */
  if(li == LONG_MIN || li == LONG_MAX)
    fprintf(stderr, "(mystrtol) Returning LONG_MIN or LONG_MAX. This might be an overflow.\n");

  return li;
}

int strtoi(char * str, int base){
  int i;
  long li;
  li = mystrtol(str, base);
  if(li < INT_MIN || li > INT_MAX){
    fprintf(stderr, "(strtoi) Failed to convert str to int.\n");
    fprintf(stderr, "Result (%ld) is too large or too small for int.\n", li);
    fprintf(stderr, "%d <= INT <= %d\n", INT_MIN, INT_MAX);
    exit(STRTOL_ERROR);
  }
  i = li;
  return i;
}

unsigned strtouint(char * str, int base){
  unsigned uint;
  long li;
  li = mystrtol(str, base);
  if(li < 0 || li > UINT_MAX){
    fprintf(stderr, "(strtoi) Failed to convert str to uint.\n");
    fprintf(stderr, "Result (%ld) is too large or too small for uint.\n", li);
    fprintf(stderr, "%u <= UINT <= %u\n", 0, UINT_MAX);
    exit(STRTOUINT_ERROR);
  }
  uint = li;
  return uint;
}

int main(int argc, char ** argv){
  unsigned uint;
  long li;
  int i;
  char read[100];
  printf("sizeof(int) = %ld\nsizeof(long) = %ld\n", sizeof(int), sizeof(long));
  printf("INT_MIN: %d, INT_MAX: %d\n", INT_MIN, INT_MAX);
  printf("UINT_MAX: %u\n", UINT_MAX);
  printf("LONG_MIN: %ld, LONG_MAX: %ld\n", LONG_MIN, LONG_MAX);
  scanf("%s", read);
  
  // i    = strtoi(read, 10);
  // uint = strtouint(read, 10);
  li   = mystrtol(read, 10);
  printf("%d %u %ld\n", i, uint, li);

  return 0;
}