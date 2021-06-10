#include <stdio.h>
#include <stdlib.h>

#define BIGGEST_INPUT 0xffff
#define ALPHABET_LENGTH 100

const char separator = ' ';

typedef struct trienode {

  unsigned freq;
  unsigned freqChildren;
  char * str;
  struct trienode ** children;
  unsigned char depth;
  unsigned char nChildren;

} TrieNode;

TrieNode * addChild(TrieNode * parent, char ch){
  int i;
  TrieNode * child = malloc(sizeof(TrieNode));
  child->depth = (parent == NULL ? 0 : parent->depth + 1);
  child->freq = 1;
  child->freqChildren = 0;
  
  child->str = calloc(child->depth, sizeof(char));
  if(parent != NULL){
    for(i = 0; i < parent->depth; i++)
      child->str[i] = parent->str[i];
    child->str[i] = ch;
  }
  child->nChildren = 0;
  child->children = calloc(ALPHABET_LENGTH, sizeof(TrieNode *));

  if(parent != NULL){
    parent->children[parent->nChildren++] = child;
    parent->freqChildren++;
  }

  return child;
}

int matchPrefix(TrieNode * node, char * s, unsigned offset){
  int i;
  for(i = 0; i < node->depth; i++)
    if(s[offset + i] != node->str[i])
      return 0;
  return 1;
}

int inChildren(TrieNode * node, char ch){
  int i;
  for(i = 0; i < node->nChildren; i++)
    if(node->children[i]->str[node->depth] == ch) // depth of parent
      return i;
  return -1;
}

void populateNode(TrieNode * node, char * s, unsigned sz){
  int i, childId;
  for(i = 0; i + node->depth < sz; i++)
    if(matchPrefix(node, s, i)){
      childId = inChildren(node, s[i + node->depth]);
      if(childId == -1)
        addChild(node, s[i + node->depth]);
      else{
        node->children[childId]->freq++;
        node->freqChildren++;
      }
    }
}

void printNode(TrieNode * node){
  int i;
  printf("depth: %u, prefix <%s> nChildren: %u, freqChildren: %u\n",
        node->depth, node->str, node->nChildren, node->freqChildren);
  for(i = 0; i < node->nChildren; i++)
    printf("child %3d: %s, freq: %3d\n", i, node->children[i]->str,
        node->children[i]->freq);
}

void populateBelow(TrieNode * node, unsigned char maxdepth,
      char * s, unsigned sz){
  int i;
  if(node->depth > maxdepth) return;
  populateNode(node, s, sz);
  for(i = 0; i < node->nChildren; i++)
    populateBelow(node->children[i], maxdepth, s, sz);
}

int countBelow(TrieNode * node, unsigned char maxdepth){
  int i, sum = 0;
  if(node->depth == maxdepth) return 1;
  for(i = 0; i < node->nChildren; i++)
    sum += countBelow(node->children[i], maxdepth);
  return sum;
}

int main(){
  char * s = calloc(BIGGEST_INPUT, sizeof(char));
  unsigned i, sz;
  scanf("%c", s);
  for(i = 1; s[i-1] != 0 && i < BIGGEST_INPUT; i++)
    scanf("%c", s+i);
  sz = i-1;
  // printf("%s\n", s);

  TrieNode * root = addChild(NULL, 0x00);
  populateBelow(root, 3, s, sz);
  for(i = 0; i <= 10; i++)
    printf("Level %d, n_nodes = %d\n", i, countBelow(root, i));

  /*
  printNode(root);
  printNode(root->children[0]);
  printNode(root->children[1]);
  printNode(root->children[2]);
  printNode(root->children[3]);
  printNode(root->children[4]);
  printNode(root->children[0]->children[0]);
  printNode(root->children[0]->children[1]);
  printNode(root->children[0]->children[2]);
  */
}