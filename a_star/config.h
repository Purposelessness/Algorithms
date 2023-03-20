#ifndef A_STAR__CONFIG_H_
#define A_STAR__CONFIG_H_

#define CHAR
#define DEBUG

#ifdef CHAR
#define TYPE char
#define IS_CHAR true
#define SYMBOL(x) char(x)
#else
#define TYPE int
#define IS_CHAR false
#define SYMBOL(x) x
#endif

#endif  // A_STAR__CONFIG_H_
