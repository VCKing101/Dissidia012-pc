/* Picks the entry for the page: the fixed entry when the lookup fails, the given one for the two special modes, otherwise the table lookup. */
typedef struct G { char p[0x1C]; unsigned flags; } G;
typedef struct S { char p[0xC8]; int f; } S;
extern int func_08AF9870(void *);
extern void *D_08B01CF8;
extern G *D_089D8140;
extern unsigned char D_089D5F20;
extern S D_09B6CDA0;
extern int func_08AFB3B4(void *);
extern void func_08900CBC(G *, void *, int);
extern void *func_088CB164(void *, int);
extern char *D_089D712C;
void *func_08AF9720(void *a, int b) {
    if (func_08AF9870(D_08B01CF8) != 0) {
        return (char *)&D_09B6CDA0 + 0x244;
    }
    if ((D_089D8140->flags & 1) != 0) {
        func_08900CBC(D_089D8140, a, b);
        return a;
    }
    if (D_089D5F20 != 0) {
        if (D_09B6CDA0.f == 5) {
            if (func_08AFB3B4(a) == 1) {
                return a;
            }
        }
    }
    return func_088CB164(D_089D712C + 0xF58, b);
}
