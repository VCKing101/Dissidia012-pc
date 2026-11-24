/* Resolves a target index (override or lookup) and moves the widget for slot `slot` to that target's position. */
extern int func_08B045F8(int slot);
extern int func_08B045D0(int idx, int which);
extern void func_088DA0A8(void *w, int pos, void *out1, void *out2);
extern void func_088D9B84(void *w, void *a);
extern void func_088D9AF4(void *w, void *a);

typedef struct B2964 { char pad[0x400]; void *w[16]; } B2964;
typedef struct S2964 { char pad[0x67C]; B2964 *blk; } S2964;

void func_08B02964(S2964 *self, int slot, int override)
{
    int cell[4];
    char big[64];
    char small[4];
    cell[0] = func_08B045F8(slot);
    if (override != -1) {
        cell[0] = override;
    }
    if (cell[0] < 0) {
        return;
    }
    cell[1] = func_08B045D0(cell[0], 0);
    cell[2] = func_08B045D0(cell[0], 1);
    func_088DA0A8(self->blk->w[cell[2]], cell[1], big, small);
    func_088D9B84(self->blk->w[slot], big);
    func_088D9AF4(self->blk->w[slot], small);
}
