/* Returns the id of the index-th entry in the given group of a table, or -1 if out of range. */
typedef struct Sub { short start; short count; } Sub;
typedef struct Rec { char pad[4]; Sub sub; } Rec;
extern void *func_08A80200(void *);
extern void *D_08A80218;
extern void *func_08AAE300(unsigned char *);
extern void *func_08AAE84C(unsigned char *);
extern Rec *func_08AAC584(void *, unsigned, void *, void *);
extern int func_08AAC66C(void *, unsigned, int);
int func_08AAEEA8(unsigned char *self, unsigned group, unsigned index) {
    void *tab;
    void *a;
    void *b;
    Rec *r;
    Sub *s;
    if (*self == 0) return -1;
    if (!(group < 3 && (int)index >= 0)) return -1;
    tab = func_08A80200(D_08A80218);
    a = func_08AAE300(self);
    b = func_08AAE84C(self);
    r = func_08AAC584(tab, group, a, b);
    if (r == 0) return -1;
    s = &r->sub;
    if (!(index < (unsigned)s->count)) return -1;
    return func_08AAC66C(tab, group, s->start + index);
}
