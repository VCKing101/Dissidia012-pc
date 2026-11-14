typedef struct PickCtx {
    char pad_0000[4];
    int field_04;
    char pad_08[0x558 - 8];
    int field_558;
    char pad_55C[0x1E70 - 0x55C];
    unsigned int field_1E70;
} PickCtx;

typedef struct PickOwner {
    char pad_0000[0x1EE4];
    PickCtx *field_1EE4;
} PickOwner;

extern void func_08AA6F0C(PickOwner *self);

int func_08AA40F8(PickOwner *self)
{
    int state;
    self->field_1EE4->field_558 = 0;
    func_08AA6F0C(self);
    state = (self->field_1EE4->field_1E70 & 2) == 0 ? 2 : 5;
    self->field_1EE4->field_04 = state;
    return 0;
}
