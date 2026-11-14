struct FlagObj {
    char pad_00[0x20];
    unsigned short field_20;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct FlagOwner {
    char pad_00[0xD8];
    FlagObj *field_D8;
};

extern "C" {

void func_08A5776C(FlagOwner *self);

void func_08A57BC4(FlagOwner *self)
{
    if (!FlagObj::has_flag(self->field_D8->field_20, 0x20)) {
        return;
    }
    func_08A5776C(self);
}

}
