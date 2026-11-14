typedef struct ValueEntry {
    char pad_000[0x78];
    float base;
    char pad_07C[0xC4];
    float floor;
} ValueEntry;

struct FloatOps {
    static float atLeast(float value, float floor)
    {
        if (value < floor) {
            value = floor;
        }
        return value;
    }
};

typedef struct ValueOwner {
    unsigned char active;
} ValueOwner;

extern "C" {

extern int D_08A80218;
extern int func_08A80200(int id);
extern int func_08AAE41C(ValueOwner *self);
extern ValueEntry *func_08AACA24(int context, int id);

float func_08AB13FC(ValueOwner *self)
{
    if (!self->active) {
        return 0.0f;
    }
    int context = func_08A80200(D_08A80218);
    int id = func_08AAE41C(self);
    ValueEntry *entry = func_08AACA24(context, id);
    if (entry == 0) {
        return 0.0f;
    }
    return FloatOps::atLeast(entry->base, entry->floor);
}

}
