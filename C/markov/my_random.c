/* DESCRIÇÃO DOS ALGORITMOS */

/*

*    https://stackoverflow.com/questions/11946622/implementation-of-random-number-generator
    
*   https://www.redhat.com/en/blog/understanding-random-number-generators-and-their-limitations-linux

*   https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c


Utilizando os algoritmos descritos acima, muito simples, conseguimos
gerar números pseudo-aleatórios de qualidade. O terceiro link explica
porque tirar o módulo de um número aleatório gerado pode gerar um viés,
e como evitá-lo.

Mas, na verdade, no caso de nossas strings, mesmo 
o mais ingênuo my_randint nos dá tanta uniformidade quanto
o mais sofisticado my_randlong_mod, que toma todos os cuidados
para não ter um intervalo enviesado. Testei com strings de 1M
caracteres, verificando a entropia com "ent" do linux.
Bom, fica a experiência.

O valor da entropia foi calculado em 5.954158 bits por byte,
muito pouco abaixo da entropia da distribuição perfeitamente uniforme,
com entropia de log2(62) = 5.954196 bits por byte
(onde 62 é o número de caracteres que podem estar nas strings)

*/

unsigned int int_seed = SEED_ORIGINAL;
unsigned long long_seed = SEED_ORIGINAL;

unsigned my_randint(){
    int_seed = int_seed * MAGIC_A + MAGIC_B;
    return int_seed;
}

unsigned long my_randlong(){
    long_seed = long_seed * MAGIC_A + MAGIC_B;
    return long_seed;
}

unsigned long my_randlong_mod(const long mod){
    unsigned long end, r;
    if ((mod - 1) == LONG_MAX) return my_randint();
    end = LONG_MAX / mod;
    end *= mod;
    while( (r = my_randlong()) >= end);
    return r % mod;
}

