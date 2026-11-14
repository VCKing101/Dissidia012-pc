typedef struct MemberFlags {
    char pad_00[0x27];
    unsigned char flags;
} MemberFlags;

void func_088F5198(MemberFlags *m)
{
    m->flags &= ~1;
}
