/* Returns the base block at 0xFC unless flag 6 is set. */
struct MemberX {
    char pad_00[0x27];
    unsigned char flags;
    char pad_28[0xFC - 0x28];
    char extra[4];

    static unsigned char hasExtra(MemberX *m)
    {
        return (m->flags & 0x40) != 0;
    }
};

extern "C" {
void *func_088F51DC(MemberX *m)
{
    if (MemberX::hasExtra(m) == 1) {
        return 0;
    }
    return m->extra;
}
}
