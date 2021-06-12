#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_random.h"

#define BIGGEST_INPUT 0xffff
#define ALPHABET_LENGTH 100
#define MAX_WORD_LENGTH 20

#define TRIE_DEPTH 3
#define SEPARATOR_CH  ' '
#define SEPARATOR_STR " "

typedef struct trienode {

  unsigned freq;
  unsigned freqChildren;
  char * str;
  struct trienode ** children;
  unsigned char depth;
  unsigned char nChildren;

} TrieNode;

TrieNode * addChild(TrieNode * parent, char ch){
  TrieNode * child = malloc(sizeof(TrieNode));
  child->depth = (parent == NULL ? 0 : parent->depth + 1);
  child->freq = 1;
  child->freqChildren = 0;
  
  child->str = calloc(child->depth, sizeof(char));
  if(parent != NULL){
    strncpy(child->str, parent->str, parent->depth);
    child->str[parent->depth] = ch;
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

TrieNode * findInNode(TrieNode * node, char ch){
  int i;
  for(i = 0; i < node->nChildren; i++)
    if(node->children[i]->str[ node->depth ] == ch)
      return node->children[i];
  return NULL;
}

TrieNode * navigateTrie(TrieNode * root, char * str){
  TrieNode * node = root;
  const int sz = strlen(str);
  int i;
  for(i = 0; i < sz; i++){
    node = findInNode(node, str[i]);
    if(node == NULL) return NULL;
  }
  return node;
}

TrieNode * getRandomChild(TrieNode * node){
  int i, sum = 0, r = randint_mod(node->freqChildren);
  if(node == NULL || node->nChildren == 0){
    fprintf(stderr,"Error: could not get random child. Node is either NULL or has no children.\n");
    return NULL;
  }
  for(i = 0; i < node->nChildren; i++){
    sum += node->children[i]->freq;
    if(r < sum)
      return node->children[i];
  }
  fprintf(stderr, "Implementation error: was not able to get a random child\n"); 
  return NULL;
}

void getRandomString(TrieNode * node, size_t length, char * ptr){
  /* Starting at node node, it will write length chars
     to pointer ptr. */
  int i;
  TrieNode * tempNode = node;
  for(i = 0; i < length; i++){
    tempNode = getRandomChild(tempNode);
    if(tempNode != NULL)
      ptr[i] = tempNode->str[tempNode->depth - 1];
  }
  ptr[i] = '\0';
}

void strSlice(char * buf, char * src, int start, int end){
  int i;
  for(i = 0; i < end-start; i++)
    buf[i] = src[i+start];
  buf[i] = '\0';
}

char * stringFromScratch(TrieNode * root){
  int len, i;
  TrieNode *node;
  char * nav = calloc(TRIE_DEPTH + 1, sizeof(char)); // to fit null-byte
  char * ptr = calloc(MAX_WORD_LENGTH + 1, sizeof(char));

  strcpy(ptr, SEPARATOR_STR);
  len = 1;

  while(len < MAX_WORD_LENGTH){
    // printf("len: %d\n", len);
    if(len > TRIE_DEPTH)
      strSlice(nav, ptr, len - TRIE_DEPTH, len);
    else
      strcpy(nav,ptr);
    
    // printf("ptr: <%s>\n", ptr);
    // printf("nav: <%s>\n", nav);

    node = navigateTrie(root, nav);
    // printNode(node);
    node = getRandomChild( node );
    // printf("randomChild: <%s>\n", node->str);

    if(node->str[node->depth - 1] == SEPARATOR_CH) break;
    ptr[len++] = node->str[node->depth - 1];
  }

  for(i = 0; i < len; i++)
    ptr[i] = ptr[i+1];
  return ptr;
}

int main(){
  char * s = calloc(BIGGEST_INPUT, sizeof(char));
  unsigned i, sz;
  scanf("%c", s);
  for(i = 1; s[i-1] != 0 && i < BIGGEST_INPUT; i++)
    scanf("%c", s+i);
  sz = i-1;

  TrieNode * root = addChild(NULL, 0x00);
  populateBelow(root, TRIE_DEPTH, s, sz);
  for(i = 0; i <= 10; i++)
    printf("Level %d, n_nodes = %d\n", i, countBelow(root, i));

  /*
  printNode(root);
  printNode(root->children[0]);
  printNode(root->children[0]->children[0]);
  printNode(root->children[0]->children[0]->children[0]);
  printNode(root->children[0]->children[0]->children[0]->children[0]);
  */

  char ptr[MAX_WORD_LENGTH];
  for(i = 0; i < 10; i++){
    getRandomString( navigateTrie(root, " "), TRIE_DEPTH, ptr);
    printf("%s\n", ptr);  
  }

  for(i = 0; i < 1000; i++){
    s = stringFromScratch(root);
    if(strlen(s) > 3){
      i++;
      printf("<%s>\n", s);
    }
  }
  /*
  TrieNode * inspect = navigateTrie(root, "con");
  TrieNode * child;
  printNode(inspect);
  for(i = 0; i < 100; i++){
    child = getRandomChild(inspect);
    printf("%c ", child->str[child->depth - 1]);
  }
  */

  return 0;
}
