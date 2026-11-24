/* Calls func_0895B98C(*a3, o, a2, word) where word is the entry at table(0x8C) + offs[cursor + k] + 0x60. */
typedef struct { int pad; unsigned cur; unsigned short *offs; } Cur08959720;
typedef struct { char pad[0x8C]; char *tab; Cur08959720 *cur; } Obj08959720;
extern void func_0895B98C(int, Obj08959720 *, int, int);
void func_08959720(int a0, Obj08959720 *o, int a2, int *a3) {
    char *tab = o->tab;
    int w = *(int *)(tab + o->cur->offs[o->cur->cur + 2] + 0x60);
    func_0895B98C(*a3, o, a2, w);
}
