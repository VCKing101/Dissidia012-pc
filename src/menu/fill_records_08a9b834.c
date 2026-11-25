/* When the block has a handle (+0x9B8), refreshes it with func_08B2BB2C, clears the five 0x18-byte records at +0x8320 and refills them from the non-zero values of the five pairs at +0x7E50 (value, state 1, flag 0). */
typedef struct Rec08A9B834 { int v; int state; int pad[3]; int flag; } Rec08A9B834;
typedef struct Pair08A9B834 { int v; int w; } Pair08A9B834;
typedef struct Blk08A9B834 {
    char pad0[0x7E50];
    Pair08A9B834 pairs[5];
    char pad1[0x8320 - 0x7E78];
    Rec08A9B834 recs[5];
} Blk08A9B834;
typedef struct S08A9B834 { char pad[0x154]; Blk08A9B834 *blk; } S08A9B834;
extern void func_08B2BB2C(int h, void *p);
void func_08A9B834(S08A9B834 *self)
{
    if (*(int *)((char *)self->blk + 0x9B8) != 0) {
        int i;
        int j;
        func_08B2BB2C(*(int *)((char *)self->blk + 0x9B8), (char *)self->blk + 0x7D4C);
        for (i = 0; i < 5; i++) {
            self->blk->recs[i].v = 0;
        }
        j = 0;
        for (i = 0; i < 5; i++) {
            int v = self->blk->pairs[i].v;
            if (v != 0) {
                self->blk->recs[j].v = v;
                self->blk->recs[j].flag = 0;
                self->blk->recs[j].state = 1;
                j++;
            }
        }
    }
}
