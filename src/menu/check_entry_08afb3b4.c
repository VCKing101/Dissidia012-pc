/* Looks up entry 0x33 and its record; if the record's kind is 2, applies the linked entry with func_088BBAB4 and func_088BA44C and returns 1; else 0. */
typedef struct { short kind; short link; } Pair;
typedef struct { char p0[0xC]; short idx; } Ent;
typedef struct { unsigned short a; char p0[3]; unsigned char b; } Rec;
extern Ent *func_088C5B34(int id);
extern char *func_088C5B90(int idx);
extern Rec *func_088BBD8C(int idx);
extern void func_088BBAB4(void *self, int a, int b, int c);
extern void func_088BA44C(void *p);
int func_08AFB3B4(char *self) {
    char *i = func_088C5B90(func_088C5B34(0x33)->idx);
    Pair *p = (Pair *)(i + 4);
    if (p->kind == 2) {
        Rec *r = func_088BBD8C(((Pair *)(i + 4))->link);
        func_088BBAB4(self, r->a, r->b, -1);
        func_088BA44C(self + self[0x773] * 0xC6 + 0x10);
        return 1;
    }
    return 0;
}
