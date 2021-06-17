#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // for downgrading long to int

#define LENUM_BUFFER_SIZE 32
#define LEPALAVRA_BUFFER_SIZE 32

#define LINHA_SEPARADORA "--------------------------------------------------\n"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

enum ERROR_TYPE{
  STRTOL_ERROR = 1,
  STRTOI_ERROR,
  STRTOUINT_ERROR,
  CONTRADICTORY_ARGS_ERROR
};

/* "safe" functions to read natural numbers */

long mystrtol(char * str, int base);

int strtoi(char * str, int base);

unsigned strtouint(char * str, int base);

/* */

void print_int_asbin(const int n);

void inverteString(char *str, int n);

int ehMinuscula(char c);

int ehMaiuscula(char c);

int strMenorIgualQue(char *p, char *q);

int strComp(char *p, char *q);

int strLen(char * s);

void zerarStr(char * s, int len);

char * newStr(char * s);

int my_pow10(int p);

int my_atoi(char * s);

void strCopy(char *a, char *b);

void strAppend(char *a, char *b);

int leNum(char * input, int * pos);

int validNum(char c);

#endif