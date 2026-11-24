/* Copies the 16-word block at +0x50 down to +0x10 in the owner's record, then calls its callback. */
typedef struct Blk58EB4 { int w[16]; } Blk58EB4;
typedef struct R58EB4 {
    char pad[0x10];
    Blk58EB4 dst;
    Blk58EB4 src;
    char pad90[0x40];
    void (*fn)(void);
} R58EB4;

typedef struct S58EB4 { R58EB4 *r; } S58EB4;

void func_08B58EB4(S58EB4 *self)
{
    R58EB4 *r = self->r;
    r->dst = r->src;
    self->r->fn();
}
