typedef struct MotionHeader {
    char pad_00[0x24];
    unsigned char kind;
    unsigned char flag;
    char pad_26[0x6];
} MotionHeader;

extern int func_08A22E2C(MotionHeader *info, unsigned int side, unsigned int slot, int motion);

unsigned char func_08A22FE0(unsigned int side, unsigned int slot, int motion)
{
    MotionHeader info;
    if (side >= 2 || slot >= 2 || motion < 0) {
        return 0;
    }
    if (func_08A22E2C(&info, side, slot, motion) != 0) {
        return 0;
    }
    return info.kind;
}
