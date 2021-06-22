#include "markov.h"

#include "macros.h"

int countBelow(TrieNode * node, uchar maxdepth){
  int i, sum = 0;
  if(node->depth == maxdepth) return 1;
  for(i = 0; i < node->nChildren; i++)
    sum += countBelow(node->children[i], maxdepth);
  return sum;
}

void printNode(TrieNode * node){
  int i;
  if(node == NULL){
    fprintf(stderr, "(printNode) Node is NULL.\n");
    return;
  }
  
  printf("-> depth: %u, <%c = 0x%x> nChildren: %u, freqChildren: %u\n",
        node->depth, node->ch, node->ch, node->nChildren, node->freqChildren);
  for(i = 0; i < node->nChildren; i++)
    printf("child[%d] <%c = 0x%x>, freq: %3d\n", i, node->children[i]->ch,
        node->children[i]->ch, node->children[i]->freq);
  printf("\n");
}

TrieNode * findInNode(TrieNode * node, uchar ch){
  int i;
  for(i = 0; i < node->nChildren; i++)
    if(node->children[i]->ch == ch)
      return node->children[i];
  return NULL;
}

TrieNode * navigateTrie(TrieNode * root, uchar * str){
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


TrieNode * addChild(TrieNode * parent, uchar ch, unsigned freq){
  TrieNode * child;
  /* if child is already in parent, update frequency and return */
  if(parent != NULL && (child = getChild(parent, ch)) != NULL){
    parent->freqChildren += freq;
    child->freq += freq;
    return child;
  }

  /* otherwise, append to parent and return */

  child = malloc(sizeof(TrieNode));
  child->depth = (parent == NULL ? 0 : parent->depth + 1);
  child->freq = freq;
  child->freqChildren = 0;
  
  child->ch = ch;
  child->nChildren = 0;
  child->children = calloc(ALPHABET_LENGTH, sizeof(TrieNode *));

  if(parent != NULL){
    parent->freqChildren += freq;
    parent->children[parent->nChildren++] = child;
  }

  return child;
}

TrieNode * getChild(TrieNode * node, uchar ch){
  int i;
  for(i = 0; i < node->nChildren; i++)
    if(node->children[i]->ch == ch)
      return node->children[i];
  return NULL;
}

void feedWordTrie(TrieNode * root, uchar * word, unsigned freq){
  int i, len;
  TrieNode * node, *rootWordStart, *rootNotWordStart;

  const int sz = strlen(word);
  if(sz == 0)
    return;

  rootWordStart = getChild(root, WORD_START);
  rootNotWordStart = getChild(root, NOT_WORD_START);
  
  node = addChild(rootWordStart, word[0], freq);
  /* in word start */
  for(len = 1; len <= trie_depth; len++){
    if(len == sz){
      addChild(node, END_OF_WORD, freq);
      break;
    }
    node = addChild(node, word[len], freq);
  }
  
  /* not in word start */
  for(i = 0; i < sz; i++){
    node = addChild(rootNotWordStart, word[i], freq);
    for(len = 1; len <= trie_depth; len++){
      if(i + len == sz){
        addChild(node, END_OF_WORD, freq);
        break;
      }
      node = addChild(node, word[i+len], freq);
    }
  }
}

uchar * stringFromTrie(TrieNode * root){
  uchar * str = calloc(MAX_WORD_LENGTH + 1, sizeof(uchar));
  uchar ch;
  int i, j;
  TrieNode * node, *rootWordStart, *rootNotWordStart;
  rootWordStart = getChild(root, WORD_START);
  rootNotWordStart = getChild(root, NOT_WORD_START);

  node = rootWordStart;
  for(i = 0; i < trie_depth; i++){
    node = getRandomChild(node);
    ch = node->ch;
    if(ch == END_OF_WORD)
      return str;
    str[i] = ch;
  }

  for(i = trie_depth; i < MAX_WORD_LENGTH; i++){
    node = rootNotWordStart;
    for(j = -trie_depth; j < 0; j++)
      node = getChild(node, str[i + j]);

    ch = getRandomChild(node)->ch;
    if(ch == END_OF_WORD)
      return str;
    str[i] = ch;
  }

  return str;
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

void strSlice(char * buf, char * src, int start, int end){
  int i;
  for(i = 0; i < end-start; i++)
    buf[i] = src[i+start];
  buf[i] = '\0';
}

Word  * newWord(uchar * s, unsigned freq){
  Word * word = malloc(sizeof(Word));
  word->s = calloc(strlen(s) + 1, sizeof(char)); // to contain null-byte
  strcpy(word->s, s);
  word->freq = freq;
  return word;
}

int inListOfWords(Word ** listOfWords, unsigned sz, uchar * s){
  int i;
  for(i = 0; i < sz; i++)
    if(strcmp(listOfWords[i]->s, s) == 0)
      return 1;
  return 0;
}

void saveTrie2(TrieNode * node, FILE * fp){
  /* each entry will hold only: ch, freq, depth*/
  int i;
  if(node == NULL)
    return;

  fwrite(&node->ch,    sizeof(uchar),    1, fp);
  fwrite(&node->depth, sizeof(uchar),    1, fp);
  fwrite(&node->freq,  sizeof(unsigned), 1, fp);
  for(i = 0; i < node->nChildren; i++) 
    saveTrie2(node->children[i], fp);
}

void saveTrie(TrieNode * node, char * path){
  FILE * fp = fopen(path, "wb");
  if(!fp){
    fprintf(stderr, "(saveTrie) Could not open file to write.\n");
    exit(FILE_IO_ERROR);
  }
  fwrite(&trie_depth, sizeof(uchar), 1, fp);
  saveTrie2(node, fp);
  fclose(fp);
}

typedef struct trienodestack {
  TrieNode **el; // element
  uchar sz;
} TrieNodeStack;

TrieNodeStack * newTrieNodeStack(){
    TrieNodeStack * st = malloc(sizeof(TrieNodeStack));
    /* we need to sum 3 here, to leave space for root, 
     * word_start vs. non_word_start, and then the leaves */
    st->el = malloc(sizeof(TrieNode *) * (trie_depth + 3));
    st->sz = 0;
    return st;
}

void free_stack(TrieNodeStack * st)
{
    free(st->el);
    free(st);
}

void triePush(TrieNodeStack * st, TrieNode * el){
  st->el[st->sz] = el;
  st->sz++;
  // unsafe
}

TrieNode * triePop(TrieNodeStack * st){
  if(st->sz < 1){
    fprintf(stderr, "(triePop) Stack is empty!\n");
    return NULL;
  }

  st->sz--;
  return st->el[st->sz];
}

TrieNode * stackLast(TrieNodeStack * st){
  return st->el[st->sz - 1];
}

int isStackEmpty(TrieNodeStack * st){
  return st->sz == 0 ? 1 : 0;
}

void printStack(TrieNodeStack * st){
  int i;
  printf("<-- PRINTING STACK (SIZE = %d) -->\n", st->sz);
  for(i = 0; i < st->sz; i++)
    printNode(st->el[i]);
  printf("<-- END OF PRINTING STACK -->\n");
}

TrieNode * readEntry(FILE * fp){
  TrieNode * child = malloc(sizeof(TrieNode));
  //child = malloc(sizeof(TrieNode));
 
  if( !(
            fread(&child->ch,        sizeof(uchar),    1, fp)
         && fread(&child->depth,     sizeof(uchar),    1, fp)
         && fread(&child->freq,      sizeof(unsigned), 1, fp)
          ))
  {
    fprintf(stderr, "(readEntry) Problem reading entry!\n");
    fprintf(stderr, "ftell %ld depth: %u <%c - 0x%x> freq: %u\n",
              ftell(fp), child->depth, child->ch, child->ch, child->freq);
    perror("Now the OS will say something fancy");
    exit(READ_ENTRY_ERROR);
  }
  child->children = calloc(ALPHABET_LENGTH, sizeof(TrieNode *));
  /* to define later, as children are read */
  child->nChildren = 0;
  child->freqChildren = 0;

  return child;
}

TrieNode * readTrie(char * path){
  FILE * fp;
  unsigned entries;
  TrieNodeStack * st;
  TrieNode * root, * parent, * child;

  /* each entry will hold only: ch, freq, depth*/
  const unsigned entrysz = sizeof(uchar) + sizeof(unsigned) + sizeof(uchar);

  fp = fopen(path, "rb");
  if(!fp){
    fprintf(stderr, "(readTrie) Could not open file to read!\n");
    perror("Error description");
    exit(FILE_IO_ERROR);
  }
  fseek(fp, 0, SEEK_END);
  const unsigned filesz = ftell(fp);
  /* remember we are leaving 1 initial byte for trie_depth */
  if((filesz - 1) % entrysz != 0){
    fprintf(stderr, "(readTrie): File size is not a multiple of entry size.\n");
    exit(SIZE_NOT_MULTIPLE_ERROR);
  }
  rewind(fp);
  const unsigned entriesInFile = (filesz - 1) / entrysz;

  if (!fread(&trie_depth, sizeof(uchar), 1, fp))
    fprintf(stderr, "Could not get trie_depth!\n");

  st = newTrieNodeStack();
  /* starting with root node */
  root = readEntry(fp);

  triePush(st, root);
  /* starting with 1 because we have already read root! */
  for(entries = 1; entries < entriesInFile; entries++){
    /* Read node and append it to last in stack.
     * Update stack depending on whether the depth of next entry is
     * higher or lower than the last entry in stack.
     *  */

    parent = stackLast(st);
    child = readEntry(fp);
    
/*
    printf("reading entry %d\n", entries);
    printStack(st);
    printNode(parent);
    printNode(child);
    */

    


    if(child->depth <= parent->depth){
      while(child->depth <= parent->depth){
        parent = triePop(st);
        //printf("pop!\n");
      }
      triePush(st, parent);
      
    }

    
    parent->children[parent->nChildren++] = child;
    parent->freqChildren += child->freq;

    triePush(st, child);
      

  }
  fclose(fp);
  free_stack(st);
  return root;
}

void freeTrie(TrieNode * root)
{
    TrieNode * node = root;
    int i;
    for (i = 0; i < node->nChildren; i++)
        freeTrie(node->children[i]);
    free(node->children);
    free(root);
}

enum ERR_GENWORDS {
    OK = 0,
    ERR_FILE_OPEN,
    ERR_WORD_TOO_LONG,
    ERR_FREQ_NOT_POSITIVE,
    ERR_WORDLIST_EMPTY,
    ERR_TIMEOUT
};

TrieNode * new_trie(){
    TrieNode * root;
    root = addChild(NULL, 0x00,    1);
    addChild(root, WORD_START,     1);
    addChild(root, NOT_WORD_START, 1);
    return root;
}

void str_from_trie_noalloc(TrieNode * root, uchar * str)
{
    uchar ch;
    int i, j;
    TrieNode * node, *rootWordStart, *rootNotWordStart;
    rootWordStart = getChild(root, WORD_START);
    rootNotWordStart = getChild(root, NOT_WORD_START);
    strcpy(str, "");

    node = rootWordStart;
    for(i = 0; i < trie_depth; i++){
        node = getRandomChild(node);
        ch = node->ch;
        if(ch == END_OF_WORD) {
            str[i] = '\0';
            return;
        }
        str[i] = ch;
    }

    for(i = trie_depth; i < MAX_WORD_LENGTH; i++){
        node = rootNotWordStart;
        for(j = -trie_depth; j < 0; j++)
            node = getChild(node, str[i + j]);
        ch = getRandomChild(node)->ch;
        if(ch == END_OF_WORD) {
            str[i] = '\0';
            return;
        }
        str[i] = ch;
    }
    str[i] = '\0';
}

void trie_info(TrieNode * root)
{
    int i;
    for (i = 0; i < 10; i++)
        printf("at depth %2d are %d nodes\n", i, countBelow(root, i));
}

int gen_words(char * filename, unsigned trdep,
unsigned n, unsigned minsz, char * answer,
unsigned timeout){
    /* This is an API to be used by NodeJS-FFI */
    /* Be aware that the thing is leaking */
    uchar *str, *buf;
    TrieNode * root;
    int varsread, ret;
    unsigned i, freq, totalwords;
    Word * word;
    Word ** wordlist;
    FILE * fp;
    unsigned long maxclocks;
    clock_t start_t;

    maxclocks = (double)timeout / 1000.0 * CLOCKS_PER_SEC;
    start_t = clock();
    fp = fopen(filename, "r");
    if (!fp) {
        ret = ERR_FILE_OPEN;
        goto err_file_open;
    }

    root = new_trie();
    trie_depth = trdep;
    wordlist = malloc(sizeof(Word *) * (MAX_WORDS_READ + n));
    str = calloc(WORD_BUFFER, sizeof(uchar));

    totalwords = 0;
    varsread = fscanf(fp, "%s %d", str, &freq);

    while (totalwords < MAX_WORDS_READ && varsread == 2) {
        if (clock() - start_t > maxclocks) {
            ret = ERR_TIMEOUT;
            goto err_file_read;
        }
        if (strlen(str) > MAX_WORD_LENGTH) {
            ret = ERR_WORD_TOO_LONG;
            goto err_file_read;
        }
        if (freq <= 0) {
            ret = ERR_FREQ_NOT_POSITIVE;
            goto err_file_read;
        }
        word = newWord(str, freq);
        wordlist[totalwords++] = word;
        feedWordTrie(root, word->s, word->freq);
        varsread = fscanf(fp, "%s %d", str, &freq);
    }

    
    if(totalwords == 0) {
        ret = ERR_WORDLIST_EMPTY;
        goto err_file_read;
    }

    time_seed();
    buf = calloc(WORD_BUFFER, sizeof(uchar));
    strcpy(answer, "");
    for(i = 0; i < n; /* */) {
        if (clock() - start_t > maxclocks) {
            ret = ERR_TIMEOUT;
            goto err_produce_output;
        }
        str_from_trie_noalloc(root, str);
        if (strlen(str) >= minsz
        && inListOfWords(wordlist, totalwords, str) == 0 ) {
            wordlist[totalwords++] = newWord(str, 1);
            sprintf(buf, "%s\n", str);
            strcat(answer, buf);
            i++;
        }
    }
    /* congratulations, if you got here it is because no error happened */
    ret = OK;
    /* cleanup */    
err_produce_output:
    free(buf);
err_file_read:
    fclose(fp);
    free(str);
    freeTrie(root);
    for (i = 0; i < totalwords; i++){
        free(wordlist[i]->s);
        free(wordlist[i]);
    }
    free(wordlist);
err_file_open:
#ifdef DEBUG_FLAG
    printf("running at %lu clocks per sec\n", CLOCKS_PER_SEC);
    printf("%lu clocks allowed\n", maxclocks);
    printf("%lu clocks elapsed\n", clock() - start_t);
#endif
    return ret;
}

/* we will just check if we ran out of time from 16 to 16 iterations */
#define CHECK_INTERVAL 0x10

int gen_words_teenprocess(char * filename, unsigned trdep,
unsigned n, unsigned minsz, unsigned timeout){
    /* This is an API to be used by NodeJS-TeenProcess */
    /* With it, we hope to fix the memory leakage */
    uchar *str;
    TrieNode * root;
    int varsread, ret;
    unsigned i, freq, totalwords;
    Word * word;
    Word ** wordlist;
    FILE * fp;
    unsigned long maxclocks;
    clock_t start_t;

    maxclocks = (double)timeout / 1000.0 * CLOCKS_PER_SEC;
    start_t = clock();
    fp = fopen(filename, "r");
    if (!fp) {
        ret = ERR_FILE_OPEN;
        goto err_file_open;
    }

    root = new_trie();
    trie_depth = trdep;
    wordlist = malloc(sizeof(Word *) * (MAX_WORDS_READ + n));
    str = calloc(WORD_BUFFER, sizeof(uchar));

    totalwords = 0;
    varsread = fscanf(fp, "%s %d", str, &freq);

    while (totalwords < MAX_WORDS_READ && varsread == 2) {
        if (totalwords % CHECK_INTERVAL && clock() - start_t > maxclocks) {
            ret = ERR_TIMEOUT;
            goto err_file_read;
        }
        if (strlen(str) > MAX_WORD_LENGTH) {
            ret = ERR_WORD_TOO_LONG;
            goto err_file_read;
        }
        if (freq <= 0) {
            ret = ERR_FREQ_NOT_POSITIVE;
            goto err_file_read;
        }
        word = newWord(str, freq);
        wordlist[totalwords++] = word;
        feedWordTrie(root, word->s, word->freq);
        varsread = fscanf(fp, "%s %d", str, &freq);
    }

    
    if(totalwords == 0) {
        ret = ERR_WORDLIST_EMPTY;
        goto err_file_read;
    }

    time_seed();
    for(i = 0; i < n; /* */) {
        if (clock() - start_t > maxclocks) {
            ret = ERR_TIMEOUT;
            goto err_produce_output;
        }
        str_from_trie_noalloc(root, str);
        if (strlen(str) >= minsz
        && inListOfWords(wordlist, totalwords, str) == 0 ) {
            wordlist[totalwords++] = newWord(str, 1);
            printf("%s\n", str);
            i++;
        }
    }
    /* congratulations, if you got here it is because no error happened */
    ret = OK;
    /* cleanup */    
err_produce_output:
    /* nothing else would happen here */
err_file_read:
    fclose(fp);
    free(str);
    freeTrie(root);
    for (i = 0; i < totalwords; i++){
        free(wordlist[i]->s);
        free(wordlist[i]);
    }
    free(wordlist);
err_file_open:
#ifdef DEBUG_FLAG
    printf("running at %lu clocks per sec\n", CLOCKS_PER_SEC);
    printf("%lu clocks allowed\n", maxclocks);
    printf("%lu clocks elapsed\n", clock() - start_t);
#endif
    return ret;
}

int main(int argc, char **argv){
    char * filename  = argv[1];
    unsigned trdep   = strtouint(argv[2], 10);
    unsigned n       = strtouint(argv[3], 10);
    unsigned minsz   = strtouint(argv[4], 10);
    unsigned timeout = strtouint(argv[5], 10);
    int ret = gen_words_teenprocess(filename, trdep, n, minsz, timeout);
    printf("Returned %d\n", ret);
    return ret;
}