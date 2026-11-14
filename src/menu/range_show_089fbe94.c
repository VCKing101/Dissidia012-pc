typedef struct RangeState {
    char pad_00000[0x33C88];
    short field_33C88;
    short field_33C8A;
} RangeState;

typedef struct RangeOwner {
    char pad_000[0x14C];
    RangeState *field_14C;
} RangeOwner;

extern void func_089FD2E8(RangeOwner *self, short first, short last);
extern void func_089FE448(RangeOwner *self);
extern void func_089FBC70(RangeOwner *self, int index);

void func_089FBE94(RangeOwner *self, int index)
{
    func_089FD2E8(self, self->field_14C->field_33C88 + 1, self->field_14C->field_33C8A);
    func_089FE448(self);
    func_089FBC70(self, index);
}
