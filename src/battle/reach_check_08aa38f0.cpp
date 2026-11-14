struct ReachRange {
    float min;
    float max;
};

struct ReachInfo {
    char pad_00[0x8];
    ReachRange range;
};

struct ReachEntry {
    char pad_00[0x28];
    float reach;
};

struct ReachCheck {
    char pad_00[0x18];
    int id;

    static float limitOf(ReachRange *range, ReachEntry *entry)
    {
        float limit = ((float *)range)[1];
        if (entry->reach < limit) {
            limit = entry->reach;
        }
        return limit;
    }
};

extern "C" {

extern int func_08AB328C(int id);
extern int func_08AA39B4(ReachCheck *self);
extern ReachEntry *func_08A7BA94(int unit, int move);
extern void *func_08AB3264(int id);
extern ReachInfo *func_08A978EC(void *slot);
extern float func_08A7B468(int unit);

/* 1 when the target is within the move's reach (capped by the unit's range). */
int func_08AA38F0(ReachCheck *self)
{
    int unit = func_08AB328C(self->id);
    ReachEntry *entry = func_08A7BA94(unit, func_08AA39B4(self));
    if (entry == 0) {
        return 0;
    }
    float limit = ReachCheck::limitOf(&func_08A978EC(func_08AB3264(self->id))->range, entry);
    if (limit < func_08A7B468(unit)) {
        return 0;
    }
    return 1;
}

}
