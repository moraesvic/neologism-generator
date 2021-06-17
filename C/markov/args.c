#include "args.h"

unsigned trie_depth;
unsigned n_generate;
int verbose, output, input;
char *filename;

static char doc[] = 
  "markovtrie: a program that takes frequency lists to build tries, and then "
  "applies a Markov process to generate new words.";

static char args_doc[] = "";

static struct argp_option options[] = {
  {"verbose",    'v', 0,      0, "Produce verbose output"},
  {"trie_depth", 'd', "N",    0, "Maximum depth the trie will go (default = 3)"},
  {"output",     'o', "FILE", 0, "Output created trie to FILE"},
  {"write",      'w', "FILE", OPTION_ALIAS},
  {"read",       'r', "FILE", 0, "Read trie from pre-existing FILE"},
  {"generate",   'g', "N",    0, "Number of random words to be generated"},
  { 0 }
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;
  unsigned tmp;

  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 'd':
    tmp = strtouint(arg, 10);
    if (tmp < 1 || tmp > 10){
      fprintf(stderr, "Trie-depth must be integer between 1 and 10.\n");
      return ARGP_ERR_CONTRADICTORY_ARGS;
    }
    arguments->trie_depth = tmp;
    break;
  case 'o': case 'w':
    arguments->output = 1;
    arguments->filename = arg;
    break;
  case 'r':
    arguments->input = 1;
    arguments->filename = arg;
    break;
  case 'g':
    arguments->n_generate = strtouint(arg, 10);
    break;

  case ARGP_KEY_ARG:
    fprintf(stderr, "Invalid option.\n");
    argp_usage (state);
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }
  if (arguments->output && arguments->input) {
    fprintf(stderr, "You either read from an existing trie-file, or read from "
    "stdin and write to a trie-file. You cannot do both at once.\n");
    
    return ARGP_ERR_CONTRADICTORY_ARGS;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int parse_args(int argc, char ** argv)
{
  struct arguments args;
  /* default values */
  args.trie_depth = 3;
  args.n_generate = 10;
  args.verbose = args.output = args.input = 0;
  args.filename = NULL;

  /* parse */
  argp_parse (&argp, argc, argv, 0, 0, &args);

  /* unpack to globals */
  trie_depth = args.trie_depth;
  n_generate = args.n_generate;
  verbose = args.verbose;
  output = args.output;
  input = args.input;
  filename = args.filename;

  /* tell user what we've just read */
  if (verbose)
    printf("trie_depth: %u, n_generate: %u, verbose: %d, output: %d "
    "input: %d, filename: %s\n", trie_depth, n_generate, verbose, output,
    input, filename);

  return 0;
}

#ifdef NONEXISTENT
int main(int argc, char ** argv){
  
  struct arguments arguments;

  /* Default values */
  arguments.verbose = 0;
  arguments.output = arguments.input = 0;
  arguments.file = NULL;
  arguments.generate = 10;

  /* */

  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf("verbose = %d, output = %d, input = %d\n"
  "file = %s\n"
  "generate = %u\n",
  arguments.verbose, arguments.output, arguments.input, 
  arguments.file,
  arguments.generate);

  return 0;
}
#endif