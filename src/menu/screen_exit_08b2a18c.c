/* Screen exit: calls the pre-step unless state flag is 1, releases every held cell, hands the block to the cleanup call and moves to state 8. Returns 0. */
struct St { int pad0; int state; char pad1[0x10]; int h18; char pad2[0x49C - 0x1C]; int cells[1]; char pad3[0xA0C - 0x4A0]; int a0c; };
struct Owner { char pad[0xA18]; struct St *st; };
extern void func_08B29688(int);
extern void func_0887BA5C(int);
extern void func_088D3C24(int, void *, void *, int);
extern int D_08B29CF8;
extern char D_08B2CB90[];
int func_08B2A18C(struct Owner *self) {
    int i;
    int off;
    if (self->st->a0c != 1) {
        func_08B29688(D_08B29CF8);
    }
    i = 0;
    off = 0;
    do {
        int v = *(int *)((char *)self->st + off + 0x49C);
        if (v != 0) {
            func_0887BA5C(v);
            *(int *)((char *)self->st + off + 0x49C) = 0;
        }
        i++;
        off += 4;
    } while (i < 0x51);
    func_088D3C24(self->st->h18, (char *)self->st + 0x1C, D_08B2CB90, 0x120);
    self->st->state = 8;
    return 0;
}
