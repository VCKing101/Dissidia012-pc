/* Copies a zero-terminated 16-bit string (length from func_08935E90, bytes via func_08935524) and terminates it; returns dst. */
extern int func_08935E90(unsigned short *);
extern void func_08935524(void *, const void *, int);
unsigned short *func_08935E30(unsigned short *dst, unsigned short *src) {
    unsigned short *s = src;
    unsigned short *d = dst;
    int n = func_08935E90(s);
    func_08935524(d, s, n + n);
    d[n] = 0;
    return d;
}
