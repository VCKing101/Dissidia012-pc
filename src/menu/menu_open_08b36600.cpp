/* Unless flag 2 is set, opens the child menu and enters state 2. */
struct Child08B36600 {
    char pad_0[0xC];
    void *field_C;
};

struct Object08B36600 {
    char pad_00[0x44];
    unsigned int field_44;
    char pad_48[0xC4 - 0x48];
    int *field_C4;
    Child08B36600 *field_C8;
};

extern "C" void func_08B53090(void *menu);

struct Flags08B36600 {
    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
    static bool closed(unsigned int flags) { return !has(flags, 2); }
};

extern "C" {
void func_08B36600(Object08B36600 *self)
{
    if (Flags08B36600::closed(self->field_44)) {
        func_08B53090(self->field_C8->field_C);
        *self->field_C4 = 2;
    }
}
}
