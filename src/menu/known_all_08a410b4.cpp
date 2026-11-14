extern "C" {
extern unsigned int *D_089D726C;
extern unsigned int D_08A455EC[];
}

struct Bits410B4 {
    static bool Test(unsigned int *set, unsigned int id) {
        return (set[id >> 5] & (1 << (id & 0x1F))) != 0;
    }
};

extern "C" void func_08A410B4(void)
{
    unsigned int *id;
    for (id = D_08A455EC; *id != 0xFFFFFFFF; id++) {
        if (Bits410B4::Test(D_089D726C, *id)) {
            continue;
        }
        return;
    }
    return;
}
