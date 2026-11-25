/* Leaves the screen: runs the base shutdown and the owner's close routines, frees the two handles at +0xD90/+0xD94 of the block, sets two block words (+0x1F898 = 5, +0x1F8A4 = 2) and returns 0. */
typedef struct Blk08B0E7B4 {
    char pad0[0x4];
    char at4[0x400];
    char pad1[0xD90 - 0x404];
    int hD90;
    int hD94;
    char pad2[0x1F898 - 0xD98];
    int w898;
    char pad3[8];
    int w8A4;
} Blk08B0E7B4;
typedef struct S08B0E7B4 { char pad[0x138]; Blk08B0E7B4 *blk; } S08B0E7B4;
extern void *D_08B01CF8;
extern int D_089D69E8;
extern void func_08B0B9E4(S08B0E7B4 *self);
extern void func_08AF7124(void *g);
extern void func_08885528(int v);
extern void func_08AF37F4(void *g, char *a, char *b);
extern void func_08AF70A4(void *g);
extern void func_08B0FD04(S08B0E7B4 *self);
extern void func_0887BA5C(int h);
int func_08B0E7B4(S08B0E7B4 *self)
{
    func_08B0B9E4(self);
    func_08AF7124(D_08B01CF8);
    func_08885528(D_089D69E8);
    func_08AF37F4(D_08B01CF8, self->blk->at4, self->blk->at4 + 0x400);
    func_08AF70A4(D_08B01CF8);
    func_08B0FD04(self);
    if (self->blk->hD90 != 0) {
        func_0887BA5C(self->blk->hD90);
        self->blk->hD90 = 0;
    }
    if (self->blk->hD94 != 0) {
        func_0887BA5C(self->blk->hD94);
        self->blk->hD94 = 0;
    }
    self->blk->w898 = 5;
    self->blk->w8A4 = 2;
    return 0;
}
