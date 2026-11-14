typedef struct MotionUnit {
    char pad_000[0x180];
    short motion;
} MotionUnit;

extern "C" bool func_08A84174(MotionUnit *unit)
{
    if (unit == 0) {
        return false;
    }
    short motion = unit->motion;
    return motion == 900 || motion == 901 || motion == 910 || motion == 911;
}
