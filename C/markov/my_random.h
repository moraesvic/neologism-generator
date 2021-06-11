extern unsigned int int_seed;
extern unsigned long long_seed;

unsigned my_randint();

#define SHUFFLE 128

/*
   A escolha do multiplicador como um número primo se deve
à sugestão dos links abaixo. Também devo a eles a macro de otimização

*  https://stackoverflow.com/questions/2624192/good-hash-function-for-strings
*  https://computinglife.wordpress.com/2008/11/20/why-do-hash-functions-use-prime-numbers/

 */

#define SEED_ORIGINAL 8589381
#define MULTIPLICADOR 31
#define MULT_31(X) ((X << 5) - X)

/* Os magic numbers podem ser compreendidos a partir dos links disponíveis
nos comentários de my_random.c */

#define MAGIC_A 1103515245
#define MAGIC_B 12345




