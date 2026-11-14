struct FlagObj {
    char pad_00[0x20];
    unsigned short field_20;

    static bool has_flag(unsigned int flags, unsigned int mask)
    {
        return (flags & mask) != 0;
    }
};

struct FlagOwner {
    char pad_00[0x48];
    FlagObj *field_48;
};

extern "C" {

void func_08A59FF4(FlagOwner *self);

void func_08A5A31C(FlagOwner *self)
{
    if (!FlagObj::has_flag(self->field_48->field_20, 0x20)) {
        return;
    }
    func_08A59FF4(self);
}

}
