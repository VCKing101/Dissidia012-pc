/* Recreates the handle of slot `slot` from its table row (with optional id overrides) and refreshes the slot. */
extern short *func_08AE4708(void);
extern void func_0887BA5C(void *h);
extern void *func_08AF39E8(void *g, int a, int b, int c, int d, int e);
extern void func_0887B924(void *h);
extern void func_08AE0AB4(void *self, int slot, int v);
extern void func_08AE0F70(void *self, int a, int slot);
extern void *D_08B01CF8;

typedef struct B0C94 { char pad[0x550]; void *h[8]; } B0C94;
typedef struct S0C94 { char pad[0x850]; B0C94 *blk; } S0C94;

void func_08AE0C94(S0C94 *self, int slot, int overA, int overB)
{
    int cell[4];
    short *row = (short *)((char *)func_08AE4708() + slot * 12);
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
    self->blk->h[slot] = func_08AF39E8(D_08B01CF8, cell[2], 0, cell[0], 0, 0);
    func_0887B924(self->blk->h[slot]);
    func_08AE0AB4(self, slot, cell[1]);
    func_08AE0F70(self, 0, slot);
}
