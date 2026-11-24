/* Recreates the text handle of slot `slot` from the given string id, then refreshes the slot and its row. */
typedef struct Row268FC { short a; short b; char pad[8]; } Row268FC;
extern Row268FC *func_08B27FF4(void);
extern void func_0887BA5C(void *h);
extern void *func_088B98FC(int id);
extern void *func_0887D868(void *g, int a, void *name, int b, int c, int d, int e, int f);
extern void func_0887B924(void *h);
extern void func_08B27A98(void *self, int slot, int v);
extern void func_08B259B8(void *self, int a, int slot, int b);
extern void *D_089D6998;

typedef struct B268FC { char pad[0xEC]; void *h[8]; } B268FC;
typedef struct S268FC { B268FC *blk; } S268FC;

void func_08B268FC(S268FC *self, int slot, int id)
{
    int pad0[1];
    int cell[1];
    void *nm;
    cell[0] = func_08B27FF4()[slot].b;
    if (self->blk->h[slot] != 0) {
        func_0887BA5C(self->blk->h[slot]);
        self->blk->h[slot] = 0;
    }
    nm = func_088B98FC(id);
    self->blk->h[slot] = func_0887D868(D_089D6998, 0, nm, -1, 0, 0, 0, 0);
    func_0887B924(self->blk->h[slot]);
    func_08B27A98(self, slot, cell[0]);
    func_08B259B8(self, 0, slot, 1);
}
