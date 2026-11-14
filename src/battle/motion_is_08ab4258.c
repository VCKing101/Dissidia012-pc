typedef struct MotionUnit {
    char pad_000[0x180];
    short motion;
} MotionUnit;

int func_08AB4258(void *self, MotionUnit *unit)
{
    short motion;
    if (unit == 0) {
        return 0;
    }
    motion = unit->motion;
    if (motion == 0x2BE || motion == 0x2BF || motion == 0x2C0 || motion == 0x2F0 || motion == 0x2F1 ||
        motion == 0x2F2 || motion == 300) {
        return 1;
    }
    return 0;
}
