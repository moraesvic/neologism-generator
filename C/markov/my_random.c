#include "my_random.h"

unsigned randint_mod(const int mod){
    unsigned end, r;
    if ((mod - 1) == INT_MAX) return rand();
    end = INT_MAX / mod;
    end *= mod;
    while( (r = rand()) >= end);
    return r % mod;
}

void time_seed(){ srand(time(NULL)); }
void set_seed(const int seed){ srand(seed); }

/* ALGORITMOS DE HASH DE TAMANHO VARIÁVEL (8 a 64 BITS) */

unsigned char hash_char(const char *s){ return hash_long(s) & CHAR_MAX; }

unsigned short hash_short(const char *s){ return hash_long(s) & SHORT_MAX; }

unsigned int hash_int(const char *s){ return hash_long(s) & INT_MAX; }

unsigned long hash_long(const char *s){
    unsigned long h_long = 0;
    while(*s)
        h_long = MULT_31(h_long) + *s++;
    return h_long;
}

/* CRIAÇÃO DE STRING ALEATÓRIA */

char *cria_random_str(const int len){
    char *s = calloc(len, sizeof(char));
    int i;
    /* deixando o último char = \0 */
    for(i = 0; i < len - 1; i++)
    {
        s[i] = randint_mod(TOTAL_RANGE);
        if(s[i] < RANGE_1) s[i] += RANGE_1_START;
        else if(s[i] < RANGE_2) s[i] += RANGE_2_DIFF;
        else if(s[i] < RANGE_3) s[i] += RANGE_3_DIFF;
    }
    return s;
}

/* */

void sorteia_array(const int n, const int r_max, int v[]){
    int i;
    for(i = 0; i < n; i++)
        v[i] = randint_mod(r_max);
}