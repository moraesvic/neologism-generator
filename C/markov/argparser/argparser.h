#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdlib.h> // strtol
#include <stdio.h>  // for IO
#include <limits.h> // for downgrading long to int

/* https://www.gnu.org/software/libc/manual/html_node/Argp.html */
#include <argp.h>


#include "macros.h"

enum OK_STATUS{
  OK_STATUS = 0
};

enum ERROR_TYPE{
  STRTOL_ERROR = 1,
  STRTOI_ERROR,
  STRTOUINT_ERROR,
  CONTRADICTORY_ARGS_ERROR
};

#endif /* ARGPARSER_H */