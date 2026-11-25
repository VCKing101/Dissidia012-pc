/* Moves the byte value to position dst in the owner's byte list (count at +0x19DF, list at +0x1D12), shifting the bytes between its old and new place. */
typedef struct Blk08AECC40 {
    char pad0[0x19DF];
    unsigned char count;
    char pad1[0x1D12 - 0x19E0];
    unsigned char list[1];
} Blk08AECC40;
typedef struct S08AECC40 { char pad[0x23F0]; Blk08AECC40 *blk; } S08AECC40;
void func_08AECC40(S08AECC40 *self, int dst, int val)
{
    int from = -1;
    int i = 0;
    Blk08AECC40 *blk = self->blk;
    for (; i < blk->count; i++) {
        if (blk->list[i] == val) {
            from = i;
            break;
        }
    }
    if (from != -1 && dst != from) {
        if (dst < from) {
            for (i = from; i > dst; i--) {
                self->blk->list[i] = self->blk->list[i - 1];
            }
        } else {
            for (i = from; i < dst; i++) {
                self->blk->list[i] = self->blk->list[i + 1];
            }
        }
        self->blk->list[dst] = val;
    }
}
