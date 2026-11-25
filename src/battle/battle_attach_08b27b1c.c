/* Creates the handle for a battle sub-object from its arguments and initialises its three parts; returns 0 or a negative error. */
typedef struct BaHandle { int pad0; int owner; } BaHandle;
typedef struct BaInfo { int pad0; int link; } BaInfo;
typedef struct BaSelf {
    unsigned char f0;
    char pad1[3];
    BaInfo *info;
    int f8;
    int fC;
    char pad10[4];
    char part14[0x10];
    char part24[0x224];
    char part248[0x68];
    char part2B0[1];
} BaSelf;
extern void func_08B27A9C(BaSelf *self);
extern BaHandle *func_088823C4(void *a, int b, int c, void *d, void *e);
extern void func_08B29AA0(void *p, int a, int b, int c, void *d);
extern void func_08B290F0(void *p, int a, int b, void *d);
extern void func_08B28730(void *p, int a, int b, void *d);
extern char D_08B27D68[];
extern char D_08B28580[];

int func_08B27B1C(BaSelf *self, void *a1, int a2, int a3, int t0, int t1, BaInfo *t2)
{
    BaHandle *h;
    int lk;
    void *p14;
    func_08B27A9C(self);
    if (a1 == 0 || t0 == 0 || t1 == 0 || t2 == 0) {
        return -1;
    }
    h = func_088823C4(a1, a2, a3, D_08B27D68, D_08B28580);
    if (h == 0) {
        return -2;
    }
    h->owner = (int)self;
    lk = t2->link;
    if (lk == 0) {
        return -3;
    }
    p14 = self->part14;
    func_08B29AA0(self->part24, t0, t1, lk, p14);
    func_08B290F0(self->part248, t1, lk, p14);
    func_08B28730(self->part2B0, t1, lk, p14);
    self->info = t2;
    self->f8 = t0;
    self->fC = 0;
    self->f0 = 1;
    return 0;
}
