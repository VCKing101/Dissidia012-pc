typedef struct SideAttach {
    unsigned char active;
    char pad_001[0x3];
    char field_04[0xFC];
    char field_100[0x4];
} SideAttach;

extern void *func_08A22C78(unsigned int side);
extern void func_08A07408(void *sub, unsigned int side);
extern void func_08A23960(void *sub);
extern void func_08B33904(void *unit, int flag);

int func_08A22328(SideAttach *self, unsigned int side)
{
    void *unit;
    if (!self->active) {
        return -1;
    }
    if (side >= 2) {
        return -2;
    }
    unit = func_08A22C78(side);
    if (unit == 0) {
        return -3;
    }
    func_08A07408(self->field_100, side);
    func_08A23960(self->field_04);
    func_08B33904(unit, 1);
    return 0;
}
