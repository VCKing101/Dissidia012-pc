/* Refreshes the page widgets: loads its resources into the page record and shows the main widget. */
extern void func_08ACCD48(void *self);
extern void func_088D46E4(void *a, int b, int c, int d, int e, int f);
extern void func_088D3F7C(void *a, void *b, const char *c, int d);
extern void func_088D3B88(void *a, void *b, const char *c, int d, int e);
extern void func_088D3CF0(void *a, void *b, const char *c);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern void func_088D9CFC(void *w);
extern const char D_08ADA194[];
extern const char D_08AD91AC[];
extern const char D_08ADA11C[];
extern const char D_08ADA14C[];

typedef struct W4F0 { char pad[0xC]; short idle; short frame; } W4F0;
typedef struct Blk0CA90 {
    char pad0[0x400];
    char at400[0x80];
    char pad480[0xC];
    void *w48C;
    char pad490[0x60];
    W4F0 *w4F0;
    char pad4F4[0x38C];
    char at880[0x960];
    char at11E0[0x2C];
    int f120C;
} Blk0CA90;
typedef struct S0CA90 { char pad[0xDB30]; Blk0CA90 *blk; } S0CA90;
typedef struct Q0CA90 { char pad[0x89C]; void *h; } Q0CA90;
typedef struct O0CA90 { char pad[0x140]; Q0CA90 *objs; } O0CA90;
extern O0CA90 *D_08B01CF8;

void func_08ACCA90(S0CA90 *self)
{
    if (self->blk->w4F0 != 0 && self->blk->w4F0->idle != -1) {
        func_08ACCD48(self);
    }
    func_088D46E4(self->blk->at11E0, self->blk->f120C, 0, 3, 0, -1);
    func_088D3F7C(D_08B01CF8->objs->h, self->blk->at880, D_08ADA194, 0);
    func_088D3B88(self->blk, self->blk->at400, D_08AD91AC, 0, 0x120);
    func_088D3CF0(self->blk, self->blk->at400, D_08ADA11C);
    self->blk->w4F0->frame = 0x33;
    func_088D428C(self->blk->at400, self->blk->at880, D_08ADA14C, 1);
    func_088D9CFC(self->blk->w48C);
}
