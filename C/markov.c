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
  saveTrie2(node, fp);
  fclose(fp);
}

typedef struct trienodestack {
  TrieNode **el; // element
  uchar sz;
} TrieNodeStack;

TrieNodeStack * newTrieNodeStack(){
    int i;
    TrieNodeStack * st = malloc(sizeof(TrieNodeStack));
    /* we need to sum 3 here, to leave space for root, 
     * word_start vs. non_word_start, and then the leaves */
    st->el = malloc(sizeof(TrieNode *) * (trie_depth + 3));
    // for(i = 0; i < trie_depth + 2; i++)
    //     st->el[i] = malloc(sizeof(TrieNode *));
    st->sz = 0;
    return st;
}

void free_stack(TrieNodeStack * st)
{
    int i;
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
  if(filesz % entrysz != 0){
    fprintf(stderr, "(readTrie): File size is not a multiple of entry size.\n");
    exit(SIZE_NOT_MULTIPLE_ERROR);
  }
  rewind(fp);
  const unsigned entriesInFile = filesz / entrysz;

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

int main(int argc, char ** argv){
    uchar * s, * newstr, *oldstr;
    TrieNode * root;
    unsigned i, freq, totalwords;
    Word * word;
    Word ** listOfWords = NULL;

    FILE *stream;
    char filenamenew[2048];

    parse_args(argc, argv);

    if (input){
        if (verbose)
                printf("Reading trie-file %s.\n", filename);
        root = readTrie(filename);
        totalwords = 0;
    } else {
        root = addChild(NULL, 0x00,    1);
        addChild(root, WORD_START,     1);
        addChild(root, NOT_WORD_START, 1);
        totalwords = 0;
        listOfWords = malloc(sizeof(Word *) * MAX_WORDS_READ);
        newstr = calloc(WORD_BUFFER, sizeof(uchar));
        oldstr = calloc(MAX_WORD_LENGTH, sizeof(uchar));
        strcpy(newstr, "");

        if (create_trie)
            stream = fopen(filename, "r");
        else
            stream = stdin;

        while( totalwords < MAX_WORDS_READ ) {
            strncpy(oldstr, newstr, MAX_WORD_LENGTH);
            fscanf(stream, "%s %d", newstr, &freq);
            if(strlen(newstr) > MAX_WORD_LENGTH)
            continue;
            if(strcmp(newstr, oldstr) == 0)
            break;
            
            word = newWord(newstr, freq);
            
            
            listOfWords[totalwords++] = word;
            feedWordTrie(root, word->s, word->freq);
        }

        free(newstr);
        free(oldstr);

        if (create_trie)
            fclose(stream);
    }

    /* trie is already built */
    if (verbose)
        for (i = 0; i < 10; i++)
            printf("at depth %2d are %d nodes\n", i, countBelow(root, i));

    if (output){
        if (verbose)
            printf("Saving trie to %s.\n", filename);
        saveTrie(root, filename);
    } else if (create_trie) {
        sprintf(filenamenew, "%s_depth=%02d_.bin", filename, trie_depth);
        if (verbose)
            printf("Saving trie to %s.\n", filenamenew);

        /* to do: save also the wordlist read from file, so
         * words generated are always new words */
        
        saveTrie(root, filenamenew);
    }

    if (n_generate){
        if (verbose)
            printf("generating list of random words\n");
        for (i = 0; i < n_generate; ){
            s = stringFromTrie(root);
            if (strlen(s) >= MIN_OUTPUT_STR
            && inListOfWords(listOfWords, totalwords, s) == 0 ) {
                i++;
                printf("<%s>\n", s);
            }
            free(s);
        }
    }

    for (i = 0; i < totalwords; i++){
        free(listOfWords[i]->s);
        free(listOfWords[i]);
    }

    if(listOfWords != NULL)
        free(listOfWords);
    freeTrie(root);
    return 0;
}
