/* Recreates the text handle of slot `slot` from the given string, then refreshes the slot and its row. */
typedef struct Row25B3C { short a; short b; char pad[8]; } Row25B3C;
extern Row25B3C *func_08B27FF4(void);
extern void func_0887BA5C(void *h);
extern void func_0887DF14(void *g, char *dst, int n, void *src);
extern void *func_0887D868(void *g, int a, void *name, unsigned b, int c, int d, int e, int f);
extern void func_0887B924(void *h);
extern void func_08B27A98(void *self, int slot, int v);
extern void func_08B259B8(void *self, int a, int slot, int b);
extern void *D_089D6998;

typedef struct B25B3C { char pad[0xEC]; void *h[8]; char pad10C[0x1E4 - 0x10C]; int z[8]; } B25B3C;
typedef struct S25B3C { B25B3C *blk; } S25B3C;

void func_08B25B3C(S25B3C *self, int slot, void *text)
{
    int pad0[1];
    int cell[1];
    char name[128];
    cell[0] = func_08B27FF4()[slot].b;
    if (self->blk->h[slot] != 0) {
        func_0887BA5C(self->blk->h[slot]);
        self->blk->h[slot] = 0;
    }
    func_0887DF14(D_089D6998, name, 0x80, text);
    self->blk->h[slot] = func_0887D868(D_089D6998, 0, name, -1, 0, 0, 0, 0);
    func_0887B924(self->blk->h[slot]);
    func_08B27A98(self, slot, cell[0]);
    func_08B259B8(self, 0, slot, 1);
    self->blk->z[slot] = 0;
}
