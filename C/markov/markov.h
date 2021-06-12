#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_random.h"


#define BIGGEST_INPUT 0xffff
#define ALPHABET_LENGTH 100
#define MAX_WORD_LENGTH 20

#define TRIE_DEPTH 3
#define SEPARATOR_CH   ' '
#define SEPARATOR_STR  " "
#define END_OF_WORD -1

typedef struct trienode {

  unsigned freq;
  unsigned freqChildren;
  char * str;
  struct trienode ** children;
  unsigned char depth;
  unsigned char nChildren;

} TrieNode;


TrieNode * addChild(TrieNode * parent, char ch);
int matchPrefix(TrieNode * node, char * s, unsigned offset);
int inChildren(TrieNode * node, char ch);
int matchEnd(TrieNode * node, char * word);
void feedWordNode(TrieNode * node, char * word, unsigned freq);
void printNode(TrieNode * node);
void populateBelow(TrieNode * node, unsigned char maxdepth,
      char * s, unsigned sz);
void feedWordBelow(TrieNode * node, unsigned char maxdepth,
      char * word, unsigned freq);
int countBelow(TrieNode * node, unsigned char maxdepth);
TrieNode * findInNode(TrieNode * node, char ch);
TrieNode * navigateTrie(TrieNode * root, char * str);
TrieNode * getRandomChild(TrieNode * node);
void getRandomString(TrieNode * node, size_t length, char * ptr);
void strSlice(char * buf, char * src, int start, int end);
char * stringFromScratchBigText(TrieNode * root);
char * stringFromScratch(TrieNode * root);
int main();

#endif /* MARKOV_H */