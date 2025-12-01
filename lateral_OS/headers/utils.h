#ifndef _DEBUG_UTILS_H_
#define _DEBUG_UTILS_H_

void char_put(char char_);
char char_get(void);

void _sleep(void);

__attribute__ ((format(printf,1,2)))
int printf(char *fmt, ...);


void *memcpy(void *dest, void *src, size_t n); 

#endif
