/* Screen exit: releases the extra resource and every held cell, hands the block to the cleanup call and moves to state 6. Returns 0. */
struct St { int pad0; int state; char pad1[0x10]; int h18; char pad2[0x49C - 0x1C]; int cells[1]; char pad3[0x6C4 - 0x4A0]; int res; };
struct Owner { char pad[0x844]; struct St *st; };
extern void func_08A80EF4(int);
extern void func_0887BA5C(int);
extern void func_088D3C24(int, void *, void *, int);
extern char D_08B2F510[];
int func_08B2D4B8(struct Owner *self) {
    int i;
    int off;
    if (self->st->res != 0) {
        func_08A80EF4(self->st->res);
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
    } while (i < 1);
    func_088D3C24(self->st->h18, (char *)self->st + 0x1C, D_08B2F510, 0x120);
    self->st->state = 6;
    return 0;
}
