/* Menu item build: loads the item window, two label tables and the cursor, with an optional flag row. */
typedef struct { char p[0xC]; short h0C; short h0E; } W;
typedef struct { char p[0x5C]; W *w5C; int f60; } O;
typedef struct {
    int f0; char p0[8]; int f0C; O *o; int f14; char a18[1];
    char p1[0x44 - 0x19]; int f44; int f48;
} It;
typedef struct { char p[0x140]; struct { char p[0x89C]; int f89C; } *g; } S;
extern char D_08B00E10[], D_08B00DB6[], D_08B00E22[], D_08B00E6A[];
extern int func_088D3B88(int, O *, int, int, int);
extern void func_088D3CF0(int, O *, void *);
extern void func_088D3F7C(int, int, void *, int);
extern void func_088D428C(O *, int, void *, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_088D9F5C(int, int, int);
extern void func_08AF63CC(S *, It *);
void func_08AF607C(S *self, It *it, unsigned char flag) {
    int k;
    if (it->o->w5C != 0) {
        if (it->o->w5C->h0C != -1) {
            func_08AF63CC(self, it);
        }
    }
    if (flag) {
        func_088D3F7C(self->g->f89C, it->f14, D_08B00E10, 0);
        k = 7;
    } else {
        k = 6;
    }
    func_088D46E4(it->a18, it->f0, 0, k, 0, -1);
    func_088D3F7C(self->g->f89C, it->f14, D_08B00DB6, 0);
    func_088D3B88(it->f0C, it->o, it->f44, 0, 0x120);
    func_088D3CF0(it->f0C, it->o, (void *)it->f48);
    func_088D3CF0(it->f0C, it->o, D_08B00E22);
    it->o->w5C->h0E = 4;
    func_088D428C(it->o, it->f14, D_08B00E6A, 1);
    if (flag) {
        func_088D9F5C(it->o->f60, 2, 1);
    }
}
