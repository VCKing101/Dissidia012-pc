/* Calls func_0895BDD4(*a3, o, a2, word) where word is the entry at table(0x8C) + offs[cursor + k] + 0x60. */
typedef struct { int pad; unsigned cur; unsigned short *offs; } Cur08959770;
typedef struct { char pad[0x8C]; char *tab; Cur08959770 *cur; } Obj08959770;
extern void func_0895BDD4(int, Obj08959770 *, int, int);
void func_08959770(int a0, Obj08959770 *o, int a2, int *a3) {
    char *tab = o->tab;
    int w = *(int *)(tab + o->cur->offs[o->cur->cur + 2] + 0x60);
    func_0895BDD4(*a3, o, a2, w);
}
