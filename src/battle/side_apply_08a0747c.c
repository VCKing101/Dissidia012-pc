typedef struct SideBlock {
    char data[0xB0];
} SideBlock;

typedef struct SideOwner {
    unsigned char active;
    char pad_001[0x223];
    SideBlock sides[2];
} SideOwner;

extern int func_08A03E1C(SideBlock *side, int value);

int func_08A0747C(SideOwner *self, unsigned int side, int value)
{
    if (!self->active) {
        return -1;
    }
    if (side >= 2) {
        return -2;
    }
    if (func_08A03E1C(&self->sides[side], value) != 0) {
        return -3;
    }
    return 0;
}
