typedef struct ItemBonus {
    short value;
} ItemBonus;

typedef struct RangeItem {
    char pad_00[0x4];
    ItemBonus bonus;
} RangeItem;

typedef struct RangeParams {
    char pad_00[0x30];
    float range;
} RangeParams;

typedef struct RangeData {
    char pad_00[0x48];
    RangeParams *params;
} RangeData;

typedef struct RangeOwner {
    char pad_00[0x38];
    int unit;
    RangeData *data;
} RangeOwner;

struct FloatMax {
    static const float &of(const float &a, const float &b) { return (a < b) ? b : a; }
};

extern "C" {

extern int func_08835778(int unit);
extern RangeItem *func_088BF4CC(int owner, int id);

float func_08AC0A40(RangeOwner *self)
{
    float range = self->data->params->range;
    RangeItem *item = func_088BF4CC(func_08835778(self->unit), 0x81);
    if (item != 0) {
        ItemBonus *bonus = &item->bonus;
        range += (float)bonus->value;
    }
    range = FloatMax::of(0.0f, range);
    return range;
}

}
