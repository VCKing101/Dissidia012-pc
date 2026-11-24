/* Resolves a target index (override or lookup) and moves the widget for slot `slot` to that target's position. */
extern int func_08B27FDC(int slot);
extern int func_08B27FB4(int idx, int which);
extern void func_088DA0A8(void *w, int pos, void *out1, void *out2);
extern void func_088D9B84(void *w, void *a);
extern void func_088D9AF4(void *w, void *a);

typedef struct B257AC { char pad[4]; void *w[16]; } B257AC;
typedef struct S257AC { B257AC *blk; } S257AC;

void func_08B257AC(S257AC *self, int slot, int override)
{
    int cell[4];
    char big[64];
    char small[4];
    cell[0] = func_08B27FDC(slot);
    if (override != -1) {
        cell[0] = override;
    }
    if (cell[0] < 0) {
        return;
    }
    cell[1] = func_08B27FB4(cell[0], 0);
    cell[2] = func_08B27FB4(cell[0], 1);
    func_088DA0A8(self->blk->w[cell[2]], cell[1], big, small);
    func_088D9B84(self->blk->w[slot], big);
    func_088D9AF4(self->blk->w[slot], small);
}
