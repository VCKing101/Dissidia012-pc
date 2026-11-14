/* Opens a random pick: draws the next 15-bit value from the battle seed. */
struct RandomPick {
    unsigned char open;
    unsigned char pad_01[3];
    int value;
};

extern int D_09AF84D0;
extern void func_08A22B58(struct RandomPick *pick);

int func_08A22B74(struct RandomPick *pick)
{
    func_08A22B58(pick);
    pick->value = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    pick->open = 1;
    return 0;
}
