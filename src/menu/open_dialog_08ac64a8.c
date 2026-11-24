/* Calls func_088FD928 with the menu's two members, constants and the strings at D_08AC740E. */
typedef struct { char p0[0x880]; int w880; char p1[0x97C - 0x884]; int w97C; } Blk;
typedef struct { char p0[0x134]; int p134; Blk *blk; } Obj;
extern int D_089D7F90;
extern const char D_08AC740E[];
extern void func_088FD928(int a, int b, int c, int d, const char *e, int f, int g, int h, int i, int j, int k, int l);
void func_08AC64A8(Obj *self) {
    func_088FD928(D_089D7F90, self->p134, 6, 0xB, D_08AC740E, 0x280A, self->blk->w880, 4, 0, 0, 0, self->blk->w97C);
}
