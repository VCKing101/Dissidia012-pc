struct KindCtx {
    char pad_0000[0x8E60];
    unsigned int field_8E60;

    static int pick(unsigned int flags)
    {
        int kind = 0;
        if ((flags & 0x8080) != 0) {
            kind = (flags & 0x100) != 0 ? 2 : 1;
        } else if ((flags & 0x400) != 0) {
            kind = (flags & 0x800) != 0 ? 4 : 3;
        }
        return kind;
    }
};

struct KindOwner {
    char pad_000[0x154];
    KindCtx *field_154;
};

extern "C" {

int func_08A1D90C(KindOwner *self)
{
    return KindCtx::pick(self->field_154->field_8E60);
}

}
