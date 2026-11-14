/* Closes the save screen parts and moves to state 7 unless a fade is running. */
struct CloseData {
    unsigned int field_00;
    int field_04;
    char pad_0008[0x6B14 - 0x8];
    int field_6B14;
    char pad_6B18[0x6B20 - 0x6B18];
    void *field_6B20;
    char pad_6B24[0x6B48 - 0x6B24];
    void *field_6B48;
    char pad_6B4C[0x7998 - 0x6B4C];
    void *field_7998;
    char field_799C[0x10];

    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

struct CloseOwner {
    char pad_00[0x58];
    CloseData *field_58;
};

extern "C" void func_08A608F0(void *dialog);
extern "C" void func_08A6A0F4(void *list);
extern "C" void func_08A6B630(void *panel, void *rect);
extern "C" void func_08A6B638(void *panel, int value);
extern "C" void func_08A51698(CloseOwner *self);
extern "C" void func_08A506D4(CloseOwner *self);

extern "C" {
void func_08A50BEC(CloseOwner *self)
{
    if (CloseData::hasFlag(self->field_58->field_00, 2)) {
        return;
    }
    func_08A608F0(self->field_58->field_6B20);
    func_08A6A0F4(self->field_58->field_6B48);
    func_08A6B630(self->field_58->field_7998, self->field_58->field_799C);
    func_08A6B638(self->field_58->field_7998, self->field_58->field_6B14);
    func_08A51698(self);
    func_08A506D4(self);
    self->field_58->field_04 = 7;
}
}
