#include "args.h"

unsigned trie_depth;
unsigned n_generate;
int verbose, output, input, create_trie;
char *filename;

static char doc[] = 
  "markovtrie: a program that takes frequency lists to build tries, and then "
  "applies a Markov process to generate new words.";

static char args_doc[] = "";

static struct argp_option options[] = {
  {"verbose",    'v', 0,      0, "Produce verbose output"},
  {"trie_depth", 'd', "N",    0, "Maximum depth the trie will go (default = 3)"},
  {"output",     'o', "FILE", 0, "Read from stdin and output created trie "
  "to FILE"},
  {"write",      'w', "FILE", OPTION_ALIAS},
  {"read",       'r', "FILE", 0, "Read trie from pre-existing FILE"},
  {"create_trie",'t', "FILE", 0, "Opens frequency list from FILE, builds "
  "trie, saves to automatically named file and closes"},
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
    case 't':
        arguments->create_trie = 1;
        arguments->filename = arg;
        arguments->n_generate = 0;
        break;

    case ARGP_KEY_ARG:
        fprintf(stderr, "Invalid option.\n");
        argp_usage (state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    if (arguments->output + arguments->input + arguments->create_trie > 1) {
        fprintf(stderr, "You either read from an existing trie-file, or read "
        "from stdin and write to a trie-file. You cannot do both at once.\n\n"
        "See also the option -t.\n");

    return ARGP_ERR_CONTRADICTORY_ARGS;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int parse_args(int argc, char ** argv)
{
    struct arguments args;
    int ret;
    /* default values */
    args.trie_depth = 3;
    args.n_generate = 10;
    args.verbose = args.output = args.input = args.create_trie = 0;
    args.filename = NULL;

    /* parse */
    ret = argp_parse (&argp, argc, argv, 0, 0, &args);
    if (ret != 0) {
        fprintf(stderr,"An error occurred reading command line arguments. "
        "Cannot proceed. Code error = %d.\n", ret);
        exit(0);
    }

    /* unpack to globals */
    trie_depth = args.trie_depth;
    n_generate = args.n_generate;
    verbose = args.verbose;
    output = args.output;
    input = args.input;
    create_trie = args.create_trie;
    filename = args.filename;

    /* tell user what we've just read */
    if (verbose)
        printf("trie_depth: %u, n_generate: %u, verbose: %d, output: %d "
        "input: %d, create_trie: %d, filename: %s\n", trie_depth, n_generate,
        verbose, output, input, create_trie, filename);

    return 0;
}