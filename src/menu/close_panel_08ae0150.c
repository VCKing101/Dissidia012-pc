/* Sets the two list positions from halfword tables, hides the lists if bit 8 of +0x5A4 is set, flags the panel closing (bit 2, timer 300), then runs the shared close calls. */
typedef struct {
    char p0[0x400]; int w400; char p1[0x438 - 0x404]; int w438; char p2[0x454 - 0x43C]; int w454;
    char p3[0x4C0 - 0x458]; int w4C0; char p4[0x5A0 - 0x4C4]; int w5A0; unsigned int f5A4;
} Blk;
typedef struct { char p0[0x850]; Blk *blk; } Obj;
extern short D_08AE45EE[];
extern int D_08B01CF8;
extern unsigned char D_089D5F20;
extern int D_089D69E8;
extern void func_088D9F5C(int a, int b, int c);
extern void func_088D9CFC(int a);
extern void func_08AF70A4(int a);
extern void func_08AF74CC(int a);
extern void func_088853CC(int a, int b);
void func_08AE0150(Obj *self) {
    Blk *b;
    func_088D9F5C(self->blk->w400, D_08AE45EE[0], 1);
    func_088D9F5C(self->blk->w4C0, D_08AE45EE[1], 1);
    b = self->blk;
    if ((b->f5A4 & 8) != 0) {
        func_088D9CFC(b->w454);
        func_088D9CFC(self->blk->w438);
        b = self->blk;
    }
    b->f5A4 = b->f5A4 | 2;
    self->blk->w5A0 = 300;
    func_08AF70A4(D_08B01CF8);
    if (D_089D5F20 == 0) {
        func_08AF74CC(D_08B01CF8);
    }
    func_088853CC(D_089D69E8, 1);
}
