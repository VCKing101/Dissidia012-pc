/* Builds the panel's lists once (when byte +0xB4E is clear), then sets flag 0x20, stores func_08ABC868's result at +0xA5C and clears byte +0xB51. */
typedef struct { char p0[0xE]; short h; } Inner;
typedef struct {
    char b0[1];
    char p0[0x400 - 1]; char b400[1];
    char p1[0x414 - 0x401]; Inner *w414;
    char p2[0x8F4 - 0x418]; char b8F4[1];
    char p3[0xA58 - 0x8F5]; unsigned int flags; int state;
    char p4[0xB4E - 0xA60]; unsigned char b4E; char p5[2]; unsigned char b51;
} Blk;
typedef struct { char p0[0x14C]; Blk *blk; } Obj;
extern char D_08AC3D3E[], D_08AC3D6E[], D_08AC4232[], D_08AC4222[];
extern void func_088D3CF0(void *a, void *b, void *c);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_088D9F5C(void *a, int b, int c);
extern void func_08ABE47C(Obj *o);
extern void func_08ABC8C4(Obj *o);
extern int func_08ABC868(Obj *o);
void func_08ABE340(Obj *self) {
    Blk *b = self->blk;
    char *buf;
    char *l;
    if (b->b4E == 0) {
        buf = b->b400;
        l = b->b8F4;
        func_088D3CF0(b, buf, D_08AC3D3E);
        func_088D3CF0(self->blk, buf, D_08AC3D6E);
        func_08ABE47C(self);
        func_088D428C(buf, l, D_08AC4232, 1);
        func_088D9F5C(self->blk->w414, 3, 1);
        self->blk->w414->h = 4;
        func_088D428C(buf, l, D_08AC4222, 1);
    }
    func_08ABC8C4(self);
    self->blk->flags = self->blk->flags | 0x20;
    self->blk->state = func_08ABC868(self);
    self->blk->b51 = 0;
}
