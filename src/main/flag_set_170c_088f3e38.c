/* Sets bit `id` of the bitmap at 0x170C. */
typedef struct FlagOwner {
    char pad_0000[0x157C];
    unsigned int seen[(0x170C - 0x157C) / 4];
    unsigned int got[1];
} FlagOwner;

void func_088F3E38(FlagOwner *owner, unsigned short id)
{
    unsigned int *word = &owner->got[(unsigned int)id >> 5];

    *word |= 1 << (id & 0x1F);
}
