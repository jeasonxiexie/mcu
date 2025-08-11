// Minimal stubs for missing libc functions
void __libc_init_array(void) {}
void _exit(int status) { while(1); }
void *memset(void *s, int c, unsigned int n) {
    unsigned char *p = s;
    while(n--) *p++ = c;
    return s;
}
void *memcpy(void *dest, const void *src, unsigned int n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while(n--) *d++ = *s++;
    return dest;
}
unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator) {
    if (denominator == 0) return 0;
    unsigned int quotient = 0;
    while (numerator >= denominator) {
        numerator -= denominator;
        quotient++;
    }
    return quotient;
}
void assert_failed(unsigned char *file, unsigned int line) {
    (void)file;
    (void)line;
}
