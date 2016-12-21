#ifndef __UTIL_H__
#define __UTIL_H__

#include <avr/io.h>

typedef void (*EventHandler) (void);

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

#endif
