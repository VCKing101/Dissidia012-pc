extern "C" {
typedef struct MotionFlags {
    unsigned int playing : 1;
    unsigned int looping : 1;
    unsigned int paused : 1;
    unsigned int reverse : 1;
    unsigned int blend : 1;
    signed int layer : 8;
    unsigned int rest : 19;
} MotionFlags;
typedef struct MotionPart { char pad_000[0x12B]; signed char flags; char pad_12C[4]; } MotionPart;
typedef struct Motion { char a[0x20]; MotionFlags flags; int f24; MotionPart *parts; } Motion;
int func_08822DA8(Motion *m)
{
    if (!m->flags.playing) {
        return 0;
    }
    int n = m->flags.layer;
    if (n == 0) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        if ((m->parts[i].flags & 1) == 0) {
            return 0;
        }
    }
    return 1;
}
}
