#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for "accept" -- checks if file has read permission
#include "my_random.h"
#include "my_string.h"
#include "macros.h"

#define ALPHABET_LENGTH 256
#define MAX_WORD_LENGTH 50
#define WORD_BUFFER     100
#define MAX_WORDS_READ  50000
#define MAX_TRIE_DEPTH  25

/* I will set control characters likely never to be used */
/* https://stackoverflow.com/questions/15464262/is-there-any-character-that-is-never-used-in-ascii-or-unicode */
/* https://en.wikipedia.org/wiki/Control_character */
#define END_OF_WORD    0x01
#define WORD_START     0x02
#define NOT_WORD_START 0x03

/* we will just check if we ran out of time from 16 to 16 iterations */
#define CHECK_INTERVAL 0x10


enum ERR {
        OK = 0,
        ERR_FILE_OPEN,
        ERR_SIZE_NOT_MULTIPLE,
        ERR_READ_ENTRY,
        ERR_WORD_TOO_LONG,
        ERR_FREQ_NOT_POSITIVE,
        ERR_WORDLIST_EMPTY,
        ERR_TIMEOUT,
        ERR_STACKOVERFLOW
};

typedef unsigned char uchar;

typedef struct trienode {
        unsigned freq;
        unsigned freqChildren;
        uchar ch;
        struct trienode ** children;
        uchar depth;
        uchar nChildren;
} TrieNode;

typedef struct {
        uchar *s;
        unsigned freq;
} Word;

typedef struct trienodestack {
        TrieNode **el; // element
        uchar sz, maxsz;
} TrieNodeStack;

/* */
uchar trie_depth;

int countBelow(TrieNode * node, uchar maxdepth);
void printNode(TrieNode * node);
void trie_info(TrieNode * root);
TrieNode * findInNode(TrieNode * node, uchar ch);
TrieNode * navigateTrie(TrieNode * root, uchar * str);
/* */
TrieNode * new_trie();
void freeTrie(TrieNode * root);
TrieNode * addChild(TrieNode * parent, uchar ch, unsigned freq);
TrieNode * getChild(TrieNode * node, uchar ch);
void feedWordTrie(TrieNode * root, uchar * word, unsigned freq);
/* */
Word  * newWord(uchar * s, unsigned freq);
int inListOfWords(Word ** listOfWords, unsigned sz, uchar * s);
/* */
TrieNode * getRandomChild(TrieNode * node);
void str_from_trie_noalloc(TrieNode * root, uchar * str);
/* */
void saveTrie(TrieNode * root, char * path);
void saveTrie2(TrieNode * node, FILE * fp);
TrieNode * readEntry(FILE * fp);
TrieNode * readTrie(char * path, uchar readuntil);
/* */
TrieNodeStack * newTrieNodeStack();
void free_stack(TrieNodeStack * st);
void triePush(TrieNodeStack * st, TrieNode * el);
TrieNode * triePop(TrieNodeStack * st);
TrieNode * stackLast(TrieNodeStack * st);
int isStackEmpty(TrieNodeStack * st);
void printStack(TrieNodeStack * st);
/* */
int gen_words_teenprocess(char * filename, unsigned trdep,
unsigned n, unsigned minsz, unsigned timeout);
/* */
int main(int argc, char ** argv);


/* --- */


#endif /* MARKOV_H */
