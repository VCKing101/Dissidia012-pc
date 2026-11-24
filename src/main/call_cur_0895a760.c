/* Calls func_0895C200(*a3, o, a2, word) where word is the entry at table(0x8C) + offs[cursor + k] + 0x60. */
typedef struct { int pad; unsigned cur; unsigned short *offs; } Cur0895A760;
typedef struct { char pad[0x8C]; char *tab; Cur0895A760 *cur; } Obj0895A760;
extern void func_0895C200(int, Obj0895A760 *, int, int);
void func_0895A760(int a0, Obj0895A760 *o, int a2, int *a3) {
    char *tab = o->tab;
    int w = *(int *)(tab + o->cur->offs[o->cur->cur + 3] + 0x60);
    func_0895C200(*a3, o, a2, w);
}
