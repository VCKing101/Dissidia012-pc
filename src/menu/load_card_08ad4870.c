typedef struct Blk { char pad0[0x95C]; int w95C; char pad960[0xF18 - 0x960]; unsigned flagsF18; char padF1C[0x1214 - 0xF1C]; int w1214; } Blk;
typedef struct QE { char pad[8]; short s8; char padA[22]; } QE;
typedef struct Q { char pad0[4]; char b4; char b5; char pad6[0xA]; QE ents[1]; } Q;
typedef struct Wk { char pad[0xDB30]; Blk *blk; } Wk;
extern int func_08ACB64C(void *);
extern int func_088B70C8(int);
extern Q *func_088C1B68(int);
extern void func_0887BA5C(int);
extern int func_08AF39B4(int *, int, int, int, int);
extern void func_088D3CF0(void *, void *, void *);
extern void func_088D3E40(void *, void *, void *);
extern void func_088D428C(void *, void *, void *, int);
extern int *D_08B01CF8;
extern int D_089D7438;
extern char D_08ADA1CA[];
extern char D_08ADA1EA[];
void func_08AD4870(Wk *self)
{
    int h = **(int **)((char *)D_08B01CF8 + 0x140);
    int cur = func_08ACB64C(self);
    int lo = 0;
    int hi = 5;
    unsigned char *e = (unsigned char *)func_088B70C8(h);
    Q *q = 0;
    if (e[8] != 0xFF) {
        q = func_088C1B68(D_089D7438);
    }
    if (q == 0 || q->b5 == 0) {
    } else {
        switch (q->b4) {
        case 1:
            lo = 0;
            hi = 5;
            break;
        case 2:
            lo = 6;
            hi = 0xB;
            break;
        case 0:
        case 3:
            lo = 0;
            hi = 0xB;
            break;
        }
        if (lo <= cur && cur <= hi) {
            QE *ents = q->ents;
            int id = ents[self->blk->w1214].s8;
            if (id != -1) {
                if (self->blk->w95C != 0) {
                    func_0887BA5C(self->blk->w95C);
                    self->blk->w95C = 0;
                }
                self->blk->w95C = func_08AF39B4(D_08B01CF8, 0, id, 0, 0);
                self->blk->flagsF18 |= 0x1000;
            }
        }
    }
    if ((self->blk->flagsF18 & 0x1000) != 0) {
        func_088D3CF0(self->blk, (char *)self->blk + 0x400, D_08ADA1CA);
        func_088D3E40(self->blk, (char *)self->blk + 0x400, D_08ADA1CA);
        func_088D428C((char *)self->blk + 0x400, (char *)self->blk + 0x880, D_08ADA1EA, 1);
    }
}
