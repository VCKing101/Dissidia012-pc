/* Enters mode 1 unless the menu is locked (flag 2): resets the lists and the view, then refreshes. */
struct ModeState {
    unsigned int field_00;
    int field_04;
    char pad_0008[0x6B14 - 0x8];
    int field_6B14;
    char pad_6B18[0x4];
    void *field_6B1C;
    void *field_6B20;
    char field_6B24[0x6B48 - 0x6B24];
    void *field_6B48;
    char pad_6B4C[0x7998 - 0x6B4C];
    void *field_7998;
};

struct ModeOwner {
    char pad_00[0x58];
    ModeState *field_58;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08A6A0F4(void *list);
void func_08A6B6CC(void *panel);
void func_08A697B0(void *view, int mode, int arg);
void func_08A6082C(void *list, char *items, int count, int start);
void func_08A60A34(void *list);
void func_08A506D4(ModeOwner *self);

void func_08A5088C(ModeOwner *self)
{
    if (ModeOwner::hasFlag(self->field_58->field_00, 2)) {
        return;
    }
    func_08A6A0F4(self->field_58->field_6B48);
    func_08A6B6CC(self->field_58->field_7998);
    func_08A697B0(self->field_58->field_6B1C, 9, -1);
    func_08A6082C(self->field_58->field_6B20, self->field_58->field_6B24, self->field_58->field_6B14, 0);
    func_08A60A34(self->field_58->field_6B20);
    func_08A506D4(self);
    self->field_58->field_04 = 1;
}
}
