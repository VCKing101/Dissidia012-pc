typedef struct RangeStats {
    char pad_00[0x14];
    short value;
} RangeStats;

typedef struct RangeUnit {
    char pad_000[0x52C];
    RangeStats *stats;
} RangeUnit;

typedef struct RangeSide {
    char pad_00[0x10];
    RangeUnit *unit;
} RangeSide;

typedef struct RangeContext {
    char pad_00[0x10];
    RangeUnit *unit;
    RangeSide *other;
} RangeContext;

class RangeBase {
public:
    int first;
    int second;
    float field_08;
    float scale;
    int own_from;
    int own_to;
    int own_extra;
    int other_from;
    int other_to;
    int other_extra;
    int field_28;
    char pad_2C[0x8];
    virtual void slot_008();
    virtual void begin(int arg, int extra, RangeContext *context);
};

class RangeTask : public RangeBase {
};

extern "C" {

void func_08AE7D48(RangeTask *self, int arg, int first, int second, int extra, RangeContext *context, float scale)
{
    int own = context->unit->stats->value;
    int other = context->other->unit->stats->value;
    self->first = first;
    self->second = second;
    self->field_08 = 0.0f;
    self->scale = scale;
    self->field_28 = 0;
    self->own_from = own;
    self->own_to = own;
    self->own_extra = 0;
    self->other_from = other;
    self->other_to = other;
    self->other_extra = 0;
    self->begin(arg, extra, context);
}

}
