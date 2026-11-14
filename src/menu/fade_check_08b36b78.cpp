/* Unless flag 0x20 is set, finishes the screen once its fade is done. */
struct State08B36B78 {
    char pad_0[0xC];
    void *field_C;
};

struct Object08B36B78 {
    char pad_00[0x44];
    unsigned int field_44;
    char pad_48[0xC4 - 0x48];
    State08B36B78 *field_C4;
};

extern "C" {
int func_08B34374(void *fade);
void func_08B37D88(Object08B36B78 *self);
}

struct Flags08B36B78 {
    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08B36B78(Object08B36B78 *self)
{
    if (!Flags08B36B78::has(self->field_44, 0x20) && func_08B34374(self->field_C4->field_C) != 0) {
        func_08B37D88(self);
    }
}
}
