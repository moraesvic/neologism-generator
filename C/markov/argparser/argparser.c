#include "argparser.h"

long mystrtol(char * str, int base){
  long li;
  char * endptr;
  if(str == NULL){
    fprintf(stderr, "(mystrtol) Passed null pointer as string.\n");
    exit(STRTOL_ERROR);
  }

  li = strtol(str, &endptr, base);

  if(endptr == str){
    fprintf(stderr, "(mystrtol) String is empty.\n");
    exit(STRTOL_ERROR);
  }

  if(*endptr != '\0'){
    fprintf(stderr, "(mystrtol) Failed to convert str to long.\n");
    fprintf(stderr, "First failed character: <%c - 0x%x>.\n", *endptr, *endptr);
    exit(STRTOL_ERROR);
  }

  /* In case the inputed value is not within the ranges of LONG,
   * it just returns LONG_MIN or LONG_MAX. 
   * Might not be desired. Fixing it for every base is not reasonable. 
   * Let's hope people are so kind as to not enter something greater
   * (or smaller) than 9223372036854775807 */
  if(li == LONG_MIN || li == LONG_MAX)
    fprintf(stderr, "(mystrtol) Returning LONG_MIN or LONG_MAX. This might be an overflow.\n");

  return li;
}

int strtoi(char * str, int base){
  int i;
  long li;
  li = mystrtol(str, base);
  if(li < INT_MIN || li > INT_MAX){
    fprintf(stderr, "(strtoi) Failed to convert str to int.\n");
    fprintf(stderr, "Result (%ld) is too large or too small for int.\n", li);
    fprintf(stderr, "%d <= INT <= %d\n", INT_MIN, INT_MAX);
    exit(STRTOI_ERROR);
  }
  i = li;
  return i;
}

unsigned strtouint(char * str, int base){
  unsigned uint;
  long li;
  li = mystrtol(str, base);
  if(li < 0 || li > UINT_MAX){
    fprintf(stderr, "(strtoi) Failed to convert str to uint.\n");
    fprintf(stderr, "Result (%ld) is too large or too small for uint.\n", li);
    fprintf(stderr, "%u <= UINT <= %u\n", 0, UINT_MAX);
    exit(STRTOUINT_ERROR);
  }
  uint = li;
  return uint;
}

static char doc[] = 
  "markovtrie: a program that takes frequency lists to build tries, and then "
  "applies a Markov process to generate new words.";

static char args_doc[] = "ARG1 ARG2";

static struct argp_option options[] = {
  {"verbose", 'v', 0,      0, "Produce verbose output"},
  {"output",  'o', "FILE", 0, "Output to FILE instead of stdout"},
  {"write",   'w', "FILE", OPTION_ALIAS},
  {"read",    'r', "FILE", 0, "Read from pre-existing trie file"},
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
    return CONTRADICTORY_ARGS_ERROR;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

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

  return OK_STATUS;
}