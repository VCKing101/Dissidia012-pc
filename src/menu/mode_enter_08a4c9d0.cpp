/* Enters mode 11 unless the menu is locked (flag 4): resets the view, the list and fades in. */
struct ModeState {
    unsigned int field_00;
    int field_04;
    char pad_0008[0x6FE8 - 0x8];
    void *field_6FE8;
    char pad_6FEC[0xC];
    void *field_6FF8;
};

struct ModeOwner {
    char pad_00[0x5C];
    ModeState *field_5C;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08A4EDC0(ModeOwner *self);
void func_08A697B0(void *view, int mode, int arg);
void func_08A4D910(ModeOwner *self);
void func_08A5B9D0(void *list, int index);
void func_08A4CFD4(ModeOwner *self, float frames);

void func_08A4C9D0(ModeOwner *self)
{
    if (ModeOwner::hasFlag(self->field_5C->field_00, 4)) {
        return;
    }
    func_08A4EDC0(self);
    func_08A697B0(self->field_5C->field_6FF8, 5, 4);
    func_08A4D910(self);
    func_08A5B9D0(self->field_5C->field_6FE8, 0);
    func_08A4CFD4(self, 0.0f);
    self->field_5C->field_04 = 0xB;
}
}
