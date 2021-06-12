#include "markov.h"

void feedWordBelow(TrieNode * node, unsigned char maxdepth,
      Word * word){
    int i;
    if(node->depth > maxdepth) return;
    feedWordNode(node, word->s, word->freq);
    for(i = 0; i < node->nChildren; i++)
      feedWordBelow(node->children[i], maxdepth, word);
}

int countBelow(TrieNode * node, unsigned char maxdepth){
  int i, sum = 0;
  if(node->depth == maxdepth) return 1;
  for(i = 0; i < node->nChildren; i++)
    sum += countBelow(node->children[i], maxdepth);
  return sum;
}

void printNode(TrieNode * node){
  int i;

  printf("depth: %u, prefix <%s> nChildren: %u, freqChildren: %u\n",
        node->depth, node->str, node->nChildren, node->freqChildren);
  for(i = 0; i < node->nChildren; i++)
    printf("child %3d: %s, freq: %3d\n", i, node->children[i]->str,
        node->children[i]->freq);
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

/*  ----- */


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

int inChildren(TrieNode * node, char ch){
  int i;
  for(i = 0; i < node->nChildren; i++)
    if(node->children[i]->str[node->depth] == ch) // depth of parent
      return i;
  return -1;
}

int matchEnd(TrieNode * node, char * word){
  int i, sz = strlen(word);

  if(node->depth == 0)
    return 0;

  if(sz < node->depth)
    return 0;

  for(i = 0; i < node->depth; i++)
    if(word[sz - node->depth + i] != node->str[i])
      return 0;
  return 1;
}

int matchBegin(TrieNode * node, char * word){
  int i, sz = strlen(word);
  if(sz < node->depth + 1)
    return 0;

  for(i = 0; i < node->depth; i++)
    if(word[i] != node->str[i])
      return 0;
  return 1;
}

int matchPrefix(TrieNode * node, char * s, unsigned offset){
  int i;
  for(i = 0; i < node->depth; i++)
    if(s[offset + i] != node->str[i])
      return 0;
  return 1;
}

void feedWordNotStart(TrieNode * notStartRoot, char * word, unsigned freq){

}

void addChildToRoot(TrieNode * root, char * word, unsigned freq){
  TrieNode * node = root;
  const int sz = strlen(word);
  char buf[TRIE_DEPTH + 1];
  int i;
  for(i = 0; i < sz - 1; i++){
    if(!inChildren(node, word[i]))
      addChild /* */
  }
}

void feedWordStart(TrieNode * startRoot, char * word, unsigned freq){
  const int sz = strlen(word);
  int i, len;
  char buf[TRIE_DEPTH + 1];
  
  /* looping in this order, we can be sure that we are following the order
     of the branches */
  for(i = 0; i < sz - 1; i++)
    for(len = 1; len <= TRIE_DEPTH && i + len < sz; len++)
      buf = strSlice(buf, word, i, i + len);
      if(navigateTrie(startRoot, buf) == NULL){
        buf = strSlice(buf, word, i, i + len - 1);
        addChild(navigateTrie(startRoot, buf), word[i + len]);
      }
    
}

void feedWordNode(TrieNode * node, char * word, unsigned freq){
  int i, childId;
  int sz = strlen(word);
  TrieNode * child;

  if(node->depth == 0){
    // printf("--- MATCH BEGIN ---\n");
    // printNode(node);
    // printf("word: %s, suffix: %c\n", word, word[0]);
    childId = inChildren(node, word[0]);
    if(childId == -1){
      child = addChild(node, word[0]);
      child->freq += freq - 1; // 1 will be already be added in addChild
      node->freqChildren += freq - 1; // same
    }
    else{
      node->children[childId]->freq += freq;
      node->freqChildren += freq;
    }
  }
  
    // printf("--- NOT MATCH BEGIN ---\n");
    // printNode(node);
    
    for(i = 0 + (!node->depth); i + node->depth < sz; i++){
      // printf("does <%s>, starting at position %d, ch %c, match the prefix?\n",
      //       word, i, word[i]);
      if(matchPrefix(node, word, i)){
        // printf("ADDED word: %s, suffix: %c\n", word, word[i + node->depth]);
        childId = inChildren(node, word[i + node->depth]);
        if(childId == -1){
          child = addChild(node, word[i + node->depth]);
          child->freq += freq - 1; // 1 will be already be added in addChild
          node->freqChildren += freq - 1; // same
        }
        else{
          node->children[childId]->freq += freq;
          node->freqChildren += freq;
        }
      }
    }

    if(matchEnd(node, word)){
      /* printf("--- MATCH END ---\n");
      printNode(node);
      printf("%s\n", word); */
      childId = inChildren(node, END_OF_WORD);
      if(childId == -1){
        child = addChild(node, END_OF_WORD);
        // this hack below is actually pretty ugly. Please change it
        // for something more maintainable
        child->freq += freq - 1; // 1 will be already be added in addChild
        node->freqChildren += freq - 1; // same
      }
      else{
        node->children[childId]->freq += freq;
        node->freqChildren += freq;
    }
  }
  
}


TrieNode * getRandomChild(TrieNode * node){
  int i;
  unsigned sum, r;
  if(node == NULL || node->nChildren == 0){
    fprintf(stderr,"Error: could not get random child. Node is either NULL or has no children.\n");
    return NULL;
  }
  sum = 0;
  r = randint_mod(node->freqChildren);
  for(i = 0; i < node->nChildren; i++){
    sum += node->children[i]->freq;
    if(r < sum)
      return node->children[i];
  }
  fprintf(stderr, "Implementation error: was not able to get a random child\n"); 
  fprintf(stderr, "freqChildren: %u, r: %u, sum: %u\n",
        node->freqChildren, r, sum);
  printNode(node);
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
  int len;
  TrieNode *node;
  char * nav = calloc(TRIE_DEPTH + 1, sizeof(char)); // to fit null-byte
  char * ptr = calloc(MAX_WORD_LENGTH, sizeof(char));

  // printf("--- STARTING stringFromScratch ---\n");

  // do{
  //   node = getRandomChild( root );
  //   // printNode(node);
  // }

  strcpy(ptr, node->str);
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

    if(node->str[node->depth - 1] == END_OF_WORD) break;
    ptr[len++] = node->str[node->depth - 1];
  }

  return ptr;
}

Word  * newWord(char * s, unsigned freq){
  Word * word = malloc(sizeof(Word));
  word->s = calloc(strlen(s) + 1, sizeof(char)); // to contain null-byte
  strcpy(word->s, s);
  word->freq = freq;
  return word;
}

int inListOfWords(Word ** listOfWords, unsigned sz, char * s){
  int i;
  for(i = 0; i < sz; i++)
    if(strcmp(listOfWords[i]->s, s) == 0)
      return 1;
  return 0;
}

int main(int argc, char ** argv){
  char * s, * word, *oldword;
  TrieNode * root;
  unsigned i, freq, totalwords;
  Word ** listOfWords;

  root = addChild(NULL, 0x00);
  addChild(root, WORD_START);
  addChild(root, NOT_WORD_START);
  totalwords = 0;

  if(argc == 2 && strcmp(argv[1], "-d") == 0){
    printf("Debug option activated.\n");
    /*
    do something special not yet implemented
    */
  }
  else{
    listOfWords = malloc(sizeof(Word *) * MAX_WORDS_READ);
    word = calloc(WORD_BUFFER, sizeof(char));
    oldword = calloc(MAX_WORD_LENGTH, sizeof(char));
    strcpy(word, "");

    while( totalwords < MAX_WORDS_READ ) {
      strncpy(oldword, word, MAX_WORD_LENGTH);
      scanf("%s %d", word, &freq);
      if(strlen(word) > MAX_WORD_LENGTH)
        continue;
      if(strcmp(oldword, word) == 0)
        break;
      
      // printf("%10d %s\n", totalwords, word);
      
      listOfWords[totalwords] = newWord(word, freq);
      feedWordBelow(root, TRIE_DEPTH, listOfWords[totalwords++]);
    }
    free(word);
    free(oldword);
  }

  s = calloc(MAX_WORD_LENGTH, sizeof(char));

  for(i = 0; i <= 10; i++)
    printf("Level %d, n_nodes = %d\n", i, countBelow(root, i));

  
  printNode(root);
  printNode(root->children[0]);
  printNode(root->children[1]);
  printNode(root->children[1]->children[0]);
  // printNode(root->children[0]->children[1]->children[0]);
  
  for(i = 0; i < 100; ){
    s = stringFromScratch(root);
    if(strlen(s) > 3 && !inListOfWords(listOfWords, totalwords, s)){
      i++;
      printf("<%s>\n", s);
    }
  }
  free(s);
  return 0;
}
