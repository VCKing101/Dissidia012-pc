typedef struct MotionUnit {
    char pad_000[0x180];
    short motion;
} MotionUnit;

int func_08AB5564(void *self, MotionUnit *unit)
{
    short motion;
    if (unit == 0) {
        return 0;
    }
    motion = unit->motion;
    if (motion == 0x32A || motion == 0x32B || motion == 0x32C || motion == 0x330 || motion == 0x32F ||
        motion == 0x32D || motion == 0x32E || motion == 0x334 || motion == 0x335 || motion == 0x336) {
        return 1;
    }
    return 0;
}
