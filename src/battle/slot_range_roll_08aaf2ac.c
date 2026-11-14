typedef struct FloatRange {
    float min;
    float max;
} FloatRange;

typedef struct RangeSlot {
    int id;
    FloatRange ranges[4];
} RangeSlot;

typedef struct RangeRoller {
    unsigned char active;
} RangeRoller;

extern int D_08A80218;
extern int D_08AA5828;
extern void *func_08A80200(int id);
extern int func_08AAEC0C(RangeRoller *self);
extern RangeSlot *func_08AAC9BC(void *owner, int id);
extern float func_08AA57D0(int id);

/* Random value inside range 0 of the roller's slot. */
float func_08AAF2AC(RangeRoller *self)
{
    RangeSlot *slot;
    float min;
    float max;

    if (self->active == 0) {
        return 0.0f;
    }
    slot = func_08AAC9BC(func_08A80200(D_08A80218), func_08AAEC0C(self));
    if (slot == 0) {
        return 0.0f;
    }
    min = slot->ranges[0].min;
    max = slot->ranges[0].max;
    return (max - min) * func_08AA57D0(D_08AA5828) + min;
}
