#ifndef ARGS_H
#define ARGS_H

#include <argp.h>
#include "my_string.h"

#ifndef ARGP_ERR_CONTRADICTORY_ARGS
#define ARGP_ERR_CONTRADICTORY_ARGS 1
#endif

/* */

extern unsigned trie_depth;
extern unsigned n_generate;
extern int verbose, output, input, create_trie;
extern char *filename;

struct arguments {
  unsigned trie_depth, n_generate;
  int verbose, output, input, create_trie;
  char *filename;
};

/* */

int parse_args(int argc, char ** argv);


#endif /* ARGS_H */
