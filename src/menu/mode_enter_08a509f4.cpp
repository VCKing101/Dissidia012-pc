/* Enters mode 4 unless the menu is locked (flag 2): closes the list, opens the grid and refreshes. */
template <class T> struct Kept { T value; Kept(T v) : value(v) {} };

struct ModeState {
    unsigned int field_00;
    int field_04;
    char pad_0008[0x6B14 - 0x8];
    int field_6B14;
    char pad_6B18[0x8];
    void *field_6B20;
    char pad_6B24[0x6B48 - 0x6B24];
    void *field_6B48;
    char field_6B4C[0x7998 - 0x6B4C];
    void *field_7998;
};

struct ModeOwner {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x58 - 0x14];
    ModeState *field_58;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08A608F0(void *list);
void func_08A6B6CC(void *panel);
void func_08A69E28(void *grid, char *items, int selected, int count, int columns, int width);
void func_08A515F0(ModeOwner *self);
void func_08A506D4(ModeOwner *self);

void func_08A509F4(ModeOwner *self)
{
    if (ModeOwner::hasFlag(self->field_58->field_00, 2)) {
        return;
    }
    Kept<ModeOwner *> saved(self);
    func_08A608F0(self->field_58->field_6B20);
    func_08A6B6CC(saved.value->field_58->field_7998);
    int mode = 4;
    func_08A69E28(saved.value->field_58->field_6B48, saved.value->field_58->field_6B4C, saved.value->field_10,
                  saved.value->field_58->field_6B14, 4, 0x6C);
    func_08A515F0(saved.value);
    func_08A506D4(saved.value);
    saved.value->field_58->field_04 = mode;
}
}
