typedef struct SideCounter {
    char pad_00[0x8];
    int used;
    int total;
} SideCounter;

typedef struct SideUnit {
    char pad_000[0x52C];
    SideCounter *counter;
} SideUnit;

struct SideUnitOps {
    static int remaining(SideUnit *unit)
    {
        int left = unit->counter->total - unit->counter->used;
        if (left < 0) {
            left = 0;
        }
        return left;
    }
};

extern "C" {

extern SideUnit *func_089E95B0(int side);
extern int func_08842F1C(int target, int count);

int func_089E99C0(unsigned int side, int target)
{
    if (side >= 2) {
        return 0;
    }
    SideUnit *unit = func_089E95B0((int)(side + 1) % 2);
    if (unit == 0) {
        return 0;
    }
    int count = SideUnitOps::remaining(unit);
    return func_08842F1C(target, count);
}

}
