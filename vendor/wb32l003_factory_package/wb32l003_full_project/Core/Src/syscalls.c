/* Minimal system calls for bare metal */

#include <stdint.h>

/* Define size_t if not already defined */
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

/* These are stub implementations - adapt as needed for your system */

void *_sbrk(int incr)
{
    extern char _end; /* Defined by the linker */
    static char *heap_end;
    char *prev_heap_end;
    
    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
    heap_end += incr;
    
    return (void *)prev_heap_end;
}

/* Stub implementations for other system calls */
int _close(int file) { return -1; }
int _fstat(int file, void *st) { return -1; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _open(const char *name, int flags, int mode) { return -1; }
int _read(int file, char *ptr, int len) { return 0; }
int _write(int file, char *ptr, int len) { return len; }

/* Memory functions if needed */
void *memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

/* Dummy exit implementation */
void _exit(int status)
{
    while (1);
}

/* Dummy abort implementation */
void abort(void)
{
    while (1);
}

/* Dummy assert implementation */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number */
    while (1);
}

/* Dummy libc init - called from startup */
void __libc_init_array(void)
{
    /* Empty - no initialization needed for bare metal */
}