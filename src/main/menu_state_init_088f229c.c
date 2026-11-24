/* Initialises the byte fields of the block at D_089D712C + 0x1DED8 (kind = a; optional extra fields when full is set), then calls func_088F20E8. */
typedef struct Blk088f229c { unsigned char pad[0x40]; unsigned char kind, b1, b2, b3, b4, b5, b6, b7; } Blk088f229c;
extern char *D_089D712C;
extern void func_088F20E8(void);
void func_088F229C(int kind, unsigned char full)
{
    Blk088f229c *b = (Blk088f229c *)(D_089D712C + 0x1DE98);
    b->kind = kind;
    b->b1 = 0;
    b->b2 = 0;
    b->b4 = 10;
    b->b7 = 0;
    if (full) {
        b->b3 = 0;
        b->b5 = 1;
        b->b6 = 0;
    }
    func_088F20E8();
}
