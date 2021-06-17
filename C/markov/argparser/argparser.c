#include <argp.h>

#ifndef ARGP_ERR_CONTRADICTORY_ARGS
#define ARGP_ERR_CONTRADICTORY_ARGS
#endif

static char doc[] = 
  "markovtrie: a program that takes frequency lists to build tries, and then "
  "applies a Markov process to generate new words.";

static char args_doc[] = "ARG1 ARG2";

static struct argp_option options[] = {
  {"verbose", 'v', 0,      0, "Produce verbose output"},
  {"output",  'o', "FILE", 0, "Output to trie-file FILE instead of stdout"},
  {"write",   'w', "FILE", OPTION_ALIAS},
  {"read",    'r', "FILE", 0, "Read from pre-existing trie-file FILE"},
  {"generate",'g', "N",    0, "Number of random words to be generated"},
  { 0 }
};

struct arguments {
  int verbose, output, input;
  char *file;
  unsigned generate;
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 'o': case 'w':
    arguments->output = 1;
    arguments->file = arg;
    break;
  case 'r':
    arguments->input = 1;
    arguments->file = arg;
    break;
  case 'g':
    arguments->generate = strtoi(arg, 10);
    break;

  case ARGP_KEY_ARG:
    printf("Invalid option.\n");
    argp_usage (state);
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }
  if (arguments->output && arguments->input) {
    printf("You either read from an existing trie-file, or read from stdin "
    "and write to a trie-file. You cannot do both at once.\n");
    
    return ARGP_ERR_CONTRADICTORY_ARGS;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

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