#ifndef MACROS_H
#define MACROS_H

/* Honestly, don't get too carried away. These things are hard to read,
 * hard to debug, type-unsafe, and overall unnecessary. */

/* Unless special clarity is needed, this should be avoided */
#define TRUE   1
#define FALSE  0

#ifdef DEBUG_FLAG
#  define DEBUGPRINT(_fmt, ...) \
    fprintf(stderr, "[%d]: " _fmt, __LINE__, __VA_ARGS__)
#else
#  define DEBUGPRINT(_fmt, ...)
#endif


/*
https://stackoverflow.com/questions/1772119/the-most-useful-user-made-c-macros-in-gcc-also-c99
*/

/* Not sure what good this would serve */
#define IMPLIES(x, y) (!(x) || (y))

#define COMPARE(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(x) COMPARE(x, 0)

/* Doesn't work for heap (malloc-allocated) arrays */
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*a))

/* Where T is the type, like int, float etc */
/* I am not really sure why the "do{...} while(0)" is necessary */
#define SWAP(x, y, T) do { T tmp = (x); (x) = (y); (y) = tmp; } while(0)
#define SORT2(a, b, T) do { if ((a) > (b)) SWAP((a), (b), T); } while (0)

#define SET(d, n, v) do{ size_t i_; for (i_ = 0; (n) - i_ > 0; ++i_) (d)[i_] = (v); } while(0)
#define ZERO(d, n) SET(d, n, 0)

/* Let's use an include between */
#define ISBETWEEN(x,a,b) ((x) >= (a) && (x) <= (b))

/* There exists a "foreach" macro, looks cool, but I don't understand it and would feel
 * confused to work with it. In case you want it, just google it. */

#endif /* MACROS_H */
