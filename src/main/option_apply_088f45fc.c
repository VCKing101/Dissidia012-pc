/* Mirrors the option byte into bit 6 of the global flags word. */
typedef struct GlobalFlags {
    unsigned int flags;
} GlobalFlags;

extern unsigned char D_089D5FD1;
extern GlobalFlags *D_089D726C;

void func_088F45FC(void)
{
    if (D_089D5FD1 != 0) {
        GlobalFlags *g = D_089D726C;

        g->flags |= 0x40;
    } else {
        GlobalFlags *g = D_089D726C;

        g->flags &= ~0x40;
    }
}
