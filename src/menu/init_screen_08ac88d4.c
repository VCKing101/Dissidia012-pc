/* Initialises the screen's block: allocates its 0x12000-byte buffer on the heap, clears its counters and tables, resets the two headers and the 0x15 slots, reads the mode, sets the flag bytes and finishes. */
typedef struct Head08AC88D4 { int a; int b; } Head08AC88D4;
typedef struct Blk08AC88D4 {
    char pad0[0xC];
    int slot[0x15];
    char pad1[0xE0 - 0x60];
    int buffer;
    char pad2[0xBB0 - 0xE4];
    Head08AC88D4 heads[2];
    char pad3[0xBE4 - 0xBC0];
    char names[0x12];
    char pad4[0xBF8 - 0xBF6];
    int mode;
    int wBFC;
    int wC00;
    char pad5[0xC14 - 0xC04];
    unsigned char bC14;
    unsigned char bC15;
} Blk08AC88D4;
typedef struct Glob08AC88D4 { char pad[0x134]; void *heap; } Glob08AC88D4;
typedef struct S08AC88D4 { char pad[0xC18]; Blk08AC88D4 *blk; } S08AC88D4;
extern Glob08AC88D4 *D_08AC7500;
extern int func_0885C6CC(void *heap, int size, int a, int b, int c, int d);
extern void func_088C0B10(int v);
extern void func_08AC7700(S08AC88D4 *self, Head08AC88D4 *h);
extern void func_08935828(char *p, int v, int n);
extern void func_08AC7578(S08AC88D4 *self);
extern int func_088F14A4(void);
extern void func_08AC86C0(S08AC88D4 *self, Blk08AC88D4 *b, int v);
extern void func_088F148C(int v);
void func_08AC88D4(S08AC88D4 *self, unsigned char flag)
{
    int i;
    Glob08AC88D4 *g = D_08AC7500;
    int zero = 0;
    {
        self->blk->buffer = func_0885C6CC(g->heap, 0x12000, 0, 0, zero, 0);
    }
    func_088C0B10(1);
    self->blk->wBFC = 0;
    self->blk->wC00 = 0;
    for (i = 0; i < 2; i++) {
        func_08AC7700(self, &self->blk->heads[i]);
    }
    for (i = 0; i < 0x15; i++) {
        self->blk->slot[i] = 0;
    }
    func_08935828(self->blk->names, 0, 0x12);
    func_08AC7578(self);
    self->blk->mode = func_088F14A4();
    {
        Blk08AC88D4 *b = self->blk;
        int big = !(b->mode < 2);
        b->bC14 = big;
        func_08AC86C0(self, b, big);
    }
    func_088F148C(0);
    self->blk->bC15 = flag;
}
