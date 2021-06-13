#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_random.h"


#define BIGGEST_INPUT 0xffff
#define ALPHABET_LENGTH 100
#define MAX_WORD_LENGTH 20
#define WORD_BUFFER 100
#define MAX_WORDS_READ 50000

#define TRIE_DEPTH 3
#define SEPARATOR_CH   ' '
#define SEPARATOR_STR  " "

/* I will set control characters likely never to be used */
/* https://stackoverflow.com/questions/15464262/is-there-any-character-that-is-never-used-in-ascii-or-unicode */
/* https://en.wikipedia.org/wiki/Control_character */
#define END_OF_WORD    0x01
#define WORD_START     0x02
#define NOT_WORD_START 0x03

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

/* */


int countBelow(TrieNode * node, uchar maxdepth);
TrieNode * findInNode(TrieNode * node, uchar ch);
TrieNode * navigateTrie(TrieNode * root, uchar * str);
TrieNode * addChild(TrieNode * parent, uchar ch, unsigned freq);
TrieNode * getChild(TrieNode * node, uchar ch);
void feedWordTrie(TrieNode * root, uchar * word, unsigned freq);
uchar * stringFromTrie(TrieNode * root);
TrieNode * getRandomChild(TrieNode * node);
void strSlice(char * buf, char * src, int start, int end);
Word  * newWord(uchar * s, unsigned freq);
int inListOfWords(Word ** listOfWords, unsigned sz, uchar * s);
int main(int argc, char ** argv);

#endif /* MARKOV_H */
