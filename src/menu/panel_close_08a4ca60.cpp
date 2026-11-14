struct CloseCtx {
    unsigned int field_00;
    int field_04;
    char pad_08[0x6FE8 - 0x8];
    int field_6FE8;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct CloseOwner {
    char pad_00[0x5C];
    CloseCtx *field_5C;
};

extern "C" {

void func_08A5BB34(int handle);
void func_08A5BC18(int handle);

void func_08A4CA60(CloseOwner *self)
{
    func_08A5BC18(self->field_5C->field_6FE8);
    if (CloseCtx::has_flag(self->field_5C->field_00, 2)) {
        return;
    }
    func_08A5BB34(self->field_5C->field_6FE8);
    self->field_5C->field_04 = 12;
}

}
