typedef struct Blk { char pad0[0x48C]; int w48C; char pad490[0x880 - 0x490]; int w880; int w884; int w888; int w88C; int w890; int w894; int w898; char pad89C[0x19B8 - 0x89C]; int w19B8; char pad19BC[0x19D4 - 0x19BC]; unsigned short s19D4[1]; char pad19D6[0x19DF - 0x19D6]; unsigned char b19DF; } Blk;
typedef struct Wk { char pad[0x23F0]; Blk *b; } Wk;
extern void func_088DA0A8(int, int, void *, void *);
extern void func_0887B8A8(int, int);
void func_08AEC550(Wk *self)
{
    char v60[64];
    struct { char pad[3]; unsigned char c; } v20;
    func_088DA0A8(self->b->w48C, 4, v60, &v20);
    if (self->b->b19DF == 0) {
        v20.c = v20.c >> 1;
        func_0887B8A8(self->b->w880, v20.c);
    } else {
        func_0887B8A8(self->b->w888, -1);
    }
    func_088DA0A8(self->b->w48C, 6, v60, &v20);
    if (self->b->s19D4[self->b->w19B8] == 0x7FFF) {
        v20.c = v20.c >> 1;
        func_0887B8A8(self->b->w888, v20.c);
    } else {
        func_0887B8A8(self->b->w888, -1);
    }
    func_088DA0A8(self->b->w48C, 3, v60, &v20);
    if (self->b->b19DF == 0) {
        v20.c = v20.c >> 1;
        func_0887B8A8(self->b->w890, v20.c);
    } else {
        func_0887B8A8(self->b->w890, -1);
    }
    func_088DA0A8(self->b->w48C, 1, v60, &v20);
    if (*(unsigned short *)((char *)self->b + self->b->w19B8 * 10 + 0x1CE0) == 0x7FFF) {
        v20.c = v20.c >> 1;
        func_0887B8A8(self->b->w898, v20.c);
    } else {
        func_0887B8A8(self->b->w898, -1);
    }
}
