/* Recreates the handle of slot `slot` from its table row (with optional id overrides) and refreshes the slot. */
extern char *func_08B27FF4(void);
extern void func_0887BA5C(void *h);
extern void *func_0887B3EC(void *heap, int a, int b, int c, int d, int e);
extern void func_0887B924(void *h);
extern void func_08B27A98(void *self, int slot, int v);
extern void func_08B259B8(void *self, int a, int slot, int b);

typedef struct B25888 { char pad[0xE8]; void *heap; void *h[8]; char pad10C[0x1E4 - 0x10C]; int z[8]; } B25888;
typedef struct S25888 { B25888 *blk; } S25888;

void func_08B25888(S25888 *self, int slot, int overA, int overB, int arg)
{
    int cell[4];
    short *row = (short *)(func_08B27FF4() + slot * 12);
    cell[0] = row[0];
    if (overA != -1) {
        cell[0] = overA;
    }
    cell[1] = row[1];
    cell[2] = row[4];
    if (overB != -1) {
        cell[1] = overB;
    }
    if (self->blk->h[slot] != 0) {
        func_0887BA5C(self->blk->h[slot]);
        self->blk->h[slot] = 0;
    }
    self->blk->h[slot] = func_0887B3EC(self->blk->heap, cell[2], 0, cell[0], 0, 0);
    func_0887B924(self->blk->h[slot]);
    func_08B27A98(self, slot, cell[1]);
    func_08B259B8(self, 0, slot, arg);
    self->blk->z[slot] = 0;
}
