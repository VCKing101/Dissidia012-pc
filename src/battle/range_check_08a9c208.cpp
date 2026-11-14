typedef struct RangeOwner {
    char pad_000[0x134];
    int level;
    char pad_138[0x28];
    int unit;
} RangeOwner;

extern "C" {
extern int D_08AA5828;
extern int func_08AB327C(int unit);
extern float func_08AB17B4(int id);
extern float func_08AA578C(int id);
}

struct RangeOps {
    static unsigned char within(int target, float limit)
    {
        int result = 0;
        if (func_08AA578C(target) < limit) {
            result = 1;
        }
        return result;
    }
};

extern "C" unsigned char func_08A9C208(RangeOwner *self)
{
    int target = D_08AA5828;
    float base = func_08AB17B4(func_08AB327C(self->unit));
    float limit = base * ((float)self->level / 5.0f + 1.0f);
    return RangeOps::within(target, limit);
}
