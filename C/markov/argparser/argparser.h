#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdlib.h> // strtol
#include <stdio.h>  // for IO
#include <limits.h> // for downgrading long to int

/* https://www.gnu.org/software/libc/manual/html_node/Argp.html */
#include <getopt.h>


#include "macros.h"

enum ERROR_TYPE{
  OK = 0,
  STRTOL_ERROR,
  STRTOI_ERROR,
  STRTOUINT_ERROR
};

#endif /* ARGPARSER_H */