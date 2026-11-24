/* Counts the page timer down; when it runs out, recreates the two icon handles from the stored name and moves to state 6. */
extern void func_0887BA5C(void *h);
extern void *func_0887D868(void *g, int a, void *name, unsigned b, int c, int d, int e, int f);
extern void *func_0887E534(void *h);
extern void func_08B09F50(void *self);
extern void *D_089D6998;

typedef struct B0E69C {
    char pad0[0xD90];
    void *hD90;
    void *hD94;
    char padD98[0x1F898 - 0xD98];
    int state;
    char pad1F89C[8];
    int timer;
    char pad1F8A8[0x1F8EC - 0x1F8A8];
    int f1F8EC;
    char pad1F8F0[0x24824 - 0x1F8F0];
    char name[1];
} B0E69C;
typedef struct S0E69C { char pad[0x138]; B0E69C *blk; } S0E69C;

int func_08B0E69C(S0E69C *self)
{
    B0E69C *b;
    self->blk->timer = self->blk->timer - 1;
    if (self->blk->timer >= 0) {
        return 0;
    }
    b = self->blk;
    if (b->hD90 != 0) {
        func_0887BA5C(b->hD90);
        self->blk->hD90 = 0;
        b = self->blk;
    }
    if (b->hD94 != 0) {
        func_0887BA5C(b->hD94);
        self->blk->hD94 = 0;
        b = self->blk;
    }
    {
        void *g = D_089D6998;
        char *nm = b->name;
        void *h = func_0887D868(g, 0, nm, -1, 0, 0, 0, 0);
        self->blk->hD90 = h;
    }
    self->blk->hD94 = func_0887E534(self->blk->hD90);
    self->blk->f1F8EC = 0;
    func_08B09F50(self);
    self->blk->state = 6;
    return 0;
}
