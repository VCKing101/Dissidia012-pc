/* Appends a record (two ids, two flag bytes and a value) to the given group's list and bumps the group's count. */
typedef struct Rec0E920 {
    short a; short b;
    int d;
    int e;
    unsigned char c0; unsigned char c1; unsigned char c2; unsigned char c3; unsigned char c4; unsigned char c5;
    short pad;
} Rec0E920;
typedef struct Grp0E920 { Rec0E920 r[800]; } Grp0E920;
typedef struct Blk0E920 { char pad[0x31BC]; int cnt[6]; char pad2[0x3264 - 0x31D4]; Grp0E920 g[6]; } Blk0E920;
typedef struct S0E920 { char pad[0x138]; Blk0E920 *blk; } S0E920;

void func_08B0E920(S0E920 *self, int grp, short a, unsigned short b, unsigned char f1, unsigned char f2, int value)
{
    int idx = self->blk->cnt[grp];
    self->blk->g[grp].r[idx].a = a;
    self->blk->g[grp].r[idx].b = b;
    self->blk->g[grp].r[idx].c4 = 0;
    self->blk->g[grp].r[idx].c5 = f1;
    self->blk->g[grp].r[idx].e = 0;
    self->blk->g[grp].r[idx].c0 = 0;
    self->blk->g[grp].r[idx].c3 = f2;
    self->blk->g[grp].r[idx].c1 = 0;
    self->blk->g[grp].r[idx].c2 = 0;
    self->blk->g[grp].r[idx].d = value;
    self->blk->cnt[grp] = self->blk->cnt[grp] + 1;
}
