struct FlagObj {
    char pad_00[0x20];
    unsigned short field_20;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct FlagOwner {
    char pad_00[0x40];
    FlagObj *field_40;
};

extern "C" {

void func_08A5CE48(FlagOwner *self);

void func_08A5D144(FlagOwner *self)
{
    if (!FlagObj::has_flag(self->field_40->field_20, 0x20)) {
        return;
    }
    func_08A5CE48(self);
}

}
