/* 1 unless the object at D_089D8364 exists and has bit 2 of its +0x10E8 word set. */
extern "C" {
struct FlagObj {
    char pad_00[0x10E8];
    unsigned flags;
    static bool Test(FlagObj *o) { return (o->flags & 4) != 0; }
};
extern FlagObj *D_089D8364;
int func_08B135E4(void)
{
    FlagObj *o = D_089D8364;
    if (o != 0) {
        int one = 1;
        if (FlagObj::Test(o) == one) {
            return 0;
        }
    }
    return 1;
}
}
