struct FlagObj {
    char pad_00[0x20];
    unsigned short field_20;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct FlagOwner {
    char pad_00[0x3C];
    FlagObj *field_3C[208];
    int field_37C;
};

extern "C" {

void func_08A5F26C(FlagOwner *self);

void func_08A5F528(FlagOwner *self)
{
    if (!FlagObj::has_flag(self->field_3C[self->field_37C]->field_20, 0x20)) {
        return;
    }
    func_08A5F26C(self);
}

}
