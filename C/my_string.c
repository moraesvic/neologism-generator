#include "my_string.h"

/* "safe" functions to read natural numbers */

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
    exit(STRTOI_ERROR);
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

/* */

void print_int_asbin(const int n){
  printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(n));
}

int ehBranco(char c){
  return c == ' ' || c == '\n';
}

int ehAlfanumerico(char c){
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9');
}

char * cortaStr(char * strOriginal, int ini, int fim){
  int i;
  char * novo;
  novo = calloc(fim - ini + 1, sizeof(char));
  for(i = ini; i < fim; i++) novo[i-ini] = strOriginal[i];
  return novo;
}

/* */

void inverteString(char *str, int n){
  char *temp = calloc(n, sizeof(char));
  int i;
  for(i = 0; i < n; i++) temp[n-1-i] = str[i];
  for(i = 0; i < n; i++) str[i] = temp[i];
  free(temp);
}

int ehMinuscula(char c){
  return c >= 'a' && c <= 'z';
}

int ehMaiuscula(char c){
  return c >= 'A' && c <= 'Z';
}

char maiParaMin(char c){
  /* recebe um char maiusculo e converte para minusculo */
  /* no ASCII, 'a' vem depois de 'A' -- ja levei isso em conta pra */
  /* nao dar overflow */
  return c + ('a' - 'A');
}

#define SAO_IGUAIS 0
#define P_MAIOR 1
#define Q_MAIOR 2

int strMenorIgualQue(char *p, char *q){
  int i, empate;
  char temp1, temp2;
  /* em caso de empate, as minusculas vao vir primeiro */
  for(i = 0; p[i] && q[i]; i++)
  {
    /* se as duas sao maiusculas ou as duas minusculas, nada a fazer */
    if( ehMaiuscula(p[i]) == ehMaiuscula(q[i]) )
      {  temp1 = p[i];  temp2 = q[i];  empate = SAO_IGUAIS;}

    else if( ehMaiuscula(p[i]) )
      {  temp1 = maiParaMin(p[i]);  temp2 = q[i];  empate = P_MAIOR;}

    else /* ou seja, q[i] é necessariamente maiúscula */
      {  temp1 = p[i];  temp2 = maiParaMin(q[i]);  empate = Q_MAIOR;}

    if(temp1 > temp2) return 0;
    else if(temp1 < temp2) return 1;
    else /* sao iguais */
    {
      if(empate == SAO_IGUAIS);  /* nada a fazer, vejamos proxima letra */
      else if(empate == P_MAIOR) return 0;
      else /* Q_MAIOR */         return 1;
    }
  }
  /* uma string mais longa vem depois no dicionario do que uma curta */
  if(p[i]) return 0;
  return 1;
}

int strComp(char *p, char *q){
    /* retorna TRUE se p == q */
    /* função verificada e debugada */
    int i;
    for(i = 0; p[i] && q[i] && p[i] == q[i]; i++);
    if(!p[i] && !q[i]) return 1;
    
    return 0;
}

int strLen(char * s){
	int i;
	for(i=0;s[i] != 0; i++);
	return i;
}

void zerarStr(char * s, int len){
	int i;
	for(i=0;i<len;i++) s[i] = 0;
}

char * newStr(char * s){
	/*Retorna um ponteiro para um nova string, idêntica à dada*/
	int i;
	for(i=0;s[i] != 0;i++);
	char * str = calloc(i+1, sizeof(char));
	for(i=0;(str[i] = s[i]);i++);
	return str;
}

int my_pow10(int p){
	if(p<=0) return 1;
	else return my_pow10(p-1)*10;
}

int my_atoi(char * s){
	int i, size, sum = 0, sign = 0;
	if(s[0] == '-') sign = 1;
	for(i=0;s[i] != 0;i++);
	size = i - 1;
	for(i=size;i>=sign;i--) sum += (s[i] - '0')*my_pow10(size-i);
	return sign==0 ? sum : -sum;
}

void strCopy(char *a, char *b){
	int i;
	for(i=0; (a[i] = b[i]) != 0; i++);
	printf("%d\n",i);
}

void strAppend(char *a, char *b){
/* Acrescenta a string em b à string em a*/
	char * temp_b = calloc(strLen(b),sizeof(char));
	int i, j;
	strCopy(temp_b,b);
	for(i=0; a[i] != 0; i++);
	for(j=0; (a[i+j] = temp_b[j]) != 0; j++);
	free(temp_b);
}

int leNum(char * input, int * pos){
	int temp;
	char * my_str = calloc(LENUM_BUFFER_SIZE, sizeof(char));

	for(; !validNum(input[*pos]) ;(*pos)++);
	temp = *pos;
	for(; validNum(input[*pos]) ;(*pos)++) my_str[*pos-temp] = input[*pos];
	temp = my_atoi(my_str);
	free(my_str);
	return temp;
}

int validNum(char c){
	return ((c >= '0' && c <= '9') || c == '-');
}

