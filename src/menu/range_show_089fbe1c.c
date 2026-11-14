typedef struct RangePair {
    short field_00;
    short field_02;
    int field_04;
} RangePair;

typedef struct RangeState {
    char pad_00000[0x2A610];
    RangePair field_2A610[1];
} RangeState;

typedef struct RangeOwner {
    char pad_000[0x14C];
    RangeState *field_14C;
} RangeOwner;

extern void func_089FD2E8(RangeOwner *self, short first, short last);
extern void func_089FE33C(RangeOwner *self);
extern void func_089FBC70(RangeOwner *self, int index);

void func_089FBE1C(RangeOwner *self, int index)
{
    func_089FD2E8(self, self->field_14C->field_2A610[index].field_00 + 1, self->field_14C->field_2A610[index].field_02);
    func_089FE33C(self);
    func_089FBC70(self, index);
}
