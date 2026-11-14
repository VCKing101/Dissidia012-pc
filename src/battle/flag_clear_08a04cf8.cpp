struct FlagTarget {
    char pad_00[0x14];
    unsigned char flags;
};

struct TargetPair {
    int key;
    FlagTarget *target;
};

struct TargetTable {
    unsigned char active;
    char pad_01[0x3];
    TargetPair entries[20];
};

extern "C" int func_08A04CF8(TargetTable *self)
{
    if (!self->active) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        TargetPair *pair = &self->entries[i];
        if (pair->target != 0) {
            pair->target->flags &= ~1;
        }
    }
    return 0;
}
