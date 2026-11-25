/* Look up a record and its sub-entry, place it into the object and register it; return 1 on success. */
typedef struct Rec { char pad[2]; signed char flag; } Rec;
typedef struct Entry { short a; short id; } Entry;
extern char *D_089D712C;
extern Rec *func_088B3524(int a);
extern Entry *func_088B3360(Rec *r, int k);
extern void func_088BBAB4(char *self, int id, int v, int arg);
extern void func_089029D4(char *p, char *q, int a, int b);
int func_08901B60(char *self, int a, int k, int arg)
{
    Rec *r = func_088B3524(a);
    Entry *e;
    if (r != 0) {
        e = func_088B3360(r, k);
        if (e != 0) {
            func_088BBAB4(self, e->id, -1, arg);
            if (r->flag >= 0) {
                func_089029D4(D_089D712C + 0x3B7A4, self + self[0x773] * 0xC6 + 0x10, r->flag, *self);
            }
            return 1;
        }
    }
    return 0;
}
