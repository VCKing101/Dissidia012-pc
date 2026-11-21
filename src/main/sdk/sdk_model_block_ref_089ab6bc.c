/* Model library: take one more reference on a memory block (u16 count at +0x0E); returns the new count. */
typedef struct D012ModelBlock {
    char unk_00[0x0E];
    unsigned short refcount; /* 0x0E */
} D012ModelBlock;

int func_089AB6BC(D012ModelBlock *block)
{
    unsigned short n;

    if (block == 0)
        return 0;
    n = block->refcount + 1;
    block->refcount = n;
    return n;
}
