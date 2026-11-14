struct PanelState {
    unsigned int field_00;
    int field_04;
    char pad_008[0x6FD4 - 0x8];
    int field_6FD4;
    char pad_6FD8[0x6FE8 - 0x6FD8];
    int field_6FE8;
    int field_6FEC;
    int pad_6FF0;
    int field_6FF4;

    static bool hasFlag(const PanelState *state, unsigned int mask) { return (state->field_00 & mask) != 0; }
};

struct PanelOwner {
    char pad_00[0x5C];
    PanelState *field_5C;
};

extern "C" void func_08A66B58(int handle);
extern "C" void func_08A5E180(int handle);

extern "C" {
void func_08A4CF00(PanelOwner *self)
{
    if (PanelState::hasFlag(self->field_5C, 2)) {
        return;
    }
    func_08A66B58(self->field_5C->field_6FEC);
    func_08A5E180(self->field_5C->field_6FF4);
    self->field_5C->field_04 = 0x14;
}
}
