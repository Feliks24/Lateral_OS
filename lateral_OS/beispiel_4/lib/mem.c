#include <stddef.h>
 
/*
 * memcpy() - primitives, nicht optimiertes memcpy()
 *
 * @dest: Ziel
 * @src: Quelle
 * @n: Menge zu kopierender Bytes
 *
 * Kompatibel zu dem memcpy() aus der "normalen" C-Library.
 */
void *memcpy(void *dest, const void *src, size_t n)
{
  	const char *s = src; 
 	char *d = dest; 
 
 	while (n--)
 		*d++ = *s++;
 
 	return dest;
}
 
