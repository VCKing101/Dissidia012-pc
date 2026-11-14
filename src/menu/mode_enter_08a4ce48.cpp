/* Enters mode 19 unless the menu is locked (flag 4): shows the selected entry's help and refreshes the panels. */
struct ModeState {
    unsigned int field_00;
    int field_04;
    char pad_0008[0x4];
    int field_0C;
    char pad_0010[0x6FEC - 0x10];
    void *field_6FEC;
    char pad_6FF0[0x4];
    void *field_6FF4;
    void *field_6FF8;
};

struct ModeOwner {
    char pad_00[0x5C];
    ModeState *field_5C;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
int func_08A66C80(void *list);
void func_08A4ED34(ModeOwner *self, int help, int text);
void func_08A697B0(void *view, int mode, int arg);
void func_08A4D910(ModeOwner *self);
void func_08A66480(void *list, int index);
void func_08A66AEC(void *list);
void func_08A5E174(void *panel);
void func_08A4CFD4(ModeOwner *self, float frames);

void func_08A4CE48(ModeOwner *self)
{
    if (ModeOwner::hasFlag(self->field_5C->field_00, 4)) {
        return;
    }
    func_08A4ED34(self, 0x19, func_08A66C80(self->field_5C->field_6FEC));
    func_08A697B0(self->field_5C->field_6FF8, 6, -1);
    func_08A4D910(self);
    func_08A66480(self->field_5C->field_6FEC, self->field_5C->field_0C);
    func_08A66AEC(self->field_5C->field_6FEC);
    func_08A5E174(self->field_5C->field_6FF4);
    func_08A4CFD4(self, 0.0f);
    self->field_5C->field_04 = 0x13;
}
}
