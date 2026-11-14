struct Work4CCAC {
    unsigned int flags;
    int step;
    char pad_08[0x6FE8 - 8];
    int part_6FE8;
    char pad_6FEC[0x6FF8 - 0x6FEC];
    int part_6FF8;
};

struct Screen4CCAC {
    char pad_00[0x5C];
    Work4CCAC *work;
};

extern "C" {
extern int D_089D69E8;
extern void func_08A5BC18(int part);
extern int func_08A5BB14(int part);
extern void func_08A6975C(int part);
extern void func_088853CC(int o, int a);
}

struct Flags4CCAC {
    static bool Has(unsigned int flags, unsigned int mask) {
        return (flags & mask) != 0;
    }
};

extern "C" void func_08A4CCAC(Screen4CCAC *self)
{
    int none = 0;
    if (Flags4CCAC::Has(self->work->flags, 4)) {
        return;
    }
    func_08A5BC18(self->work->part_6FE8);
    if (func_08A5BB14(self->work->part_6FE8) == 0) {
        return;
    }
    func_08A6975C(self->work->part_6FF8);
    func_088853CC(D_089D69E8, none);
    self->work->step = 0xF;
}
