/* Picks a slot from a lookup table plus an offset and forwards to func_08ACDE24 with the packed nibbles, repeating for the next slot when the pair flag is set. */
typedef struct U8CDEF4 { char pad[0x16]; char c; } U8CDEF4;
extern int D_08ADA4D0[];
extern void func_08ACDE24(void *self, int slot, U8CDEF4 *u, int s, int t, unsigned char f, int code);
void func_08ACDEF4(void *self, int idx, int off, int hi, unsigned char pair, U8CDEF4 *u, short s, int t, unsigned char f)
{
    int slot = D_08ADA4D0[idx] + off;
    int c = u->c & 0xF;
    if (slot >= 0 && slot < 4) {
        unsigned char code = c | (hi << 4);
        func_08ACDE24(self, slot, u, s, t, f, code);
        if (pair != 0) {
            func_08ACDE24(self, slot + 1, u, s, t, f, code);
        }
    } else {
        func_08ACDE24(self, slot, u, s, t, f, c);
    }
}
