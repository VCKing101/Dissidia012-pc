typedef struct PickPair {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
} PickPair;

typedef struct PickLimits {
    char pad_00[0x10];
    short field_10;
} PickLimits;

typedef struct PickState {
    char pad_00[0x40];
    PickLimits *field_40;
} PickState;

typedef struct PickOwner {
    char pad_000[0x14C];
    PickState *field_14C;
} PickOwner;

extern PickPair D_08A0B240[];

unsigned short func_089EF7E8(PickOwner *self, signed char index, unsigned char second)
{
    unsigned short result = 0xFFFF;
    if (index < self->field_14C->field_40->field_10) {
        if (second) {
            result = D_08A0B240[index].field_02;
        } else {
            result = D_08A0B240[index].field_00;
        }
    }
    return result;
}
