#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include <stdlib.h>

/* 
 * Even though we find a RNG in stdlib, it is not cryptographically
 * secure. Ideally, we should implement later with libsodium.
 * 
 * https://github.com/jedisct1/libsodium
 * 
 *  */

#include <time.h>
void time_seed();
void set_seed(const int seed);

/* Maiores valores possíveis para cada um dos tipos */

#define CHAR_MAX  0xffU
#define SHORT_MAX 0xffffU
#define INT_MAX   0xffffffffU
#define LONG_MAX  0xffffffffffffffffU

unsigned randint_mod(const int mod);

/* ALGORITMOS DE HASH DE TAMANHO VARIÁVEL (8 a 64 BITS) */

#define MULT_31(X) ((X << 5) - X)

unsigned char  hash_char(const char *s);
unsigned short hash_short(const char *s);
unsigned int   hash_int(const char *s);
unsigned long  hash_long(const char *s);

/* CRIAÇÃO DE STRING ALEATÓRIA */

#define ORD_0 48
#define ORD_9 57
#define ORD_A 65
#define ORD_Z 90
#define ORD_a 97
#define ORD_z 122
#define TOTAL_RANGE (ORD_9 - ORD_0 + 1 + \
                     ORD_Z - ORD_A + 1 + \
                     ORD_z - ORD_a + 1)
#define RANGE_1 (ORD_9 - ORD_0 + 1)
#define RANGE_1_START ORD_0
#define RANGE_2 (RANGE_1 + ORD_Z - ORD_A + 1)
#define RANGE_2_START ORD_A
#define RANGE_2_DIFF RANGE_2_START - RANGE_1
#define RANGE_3 (RANGE_2 + ORD_z - ORD_a + 1)
#define RANGE_3_START ORD_a
#define RANGE_3_DIFF RANGE_3_START - RANGE_2

char *cria_random_str(const int len);

void sorteia_array(const int n, const int r_max, int v[]);

#endif