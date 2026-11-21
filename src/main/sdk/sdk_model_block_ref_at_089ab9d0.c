/* Model library: if p was allocated (non-null), take one more reference on the heap's current block
 * (blocks[heap], func_089AB6BC); returns p. */
typedef struct D012ModelBlock {
    char unk_00[0x0E];
    unsigned short refcount; /* 0x0E */
} D012ModelBlock;

int func_089AB6BC(D012ModelBlock *block);

void *func_089AB9D0(D012ModelBlock **blocks, int heap, void *p)
{
    if (blocks != 0) {
        if (p != 0)
            func_089AB6BC(blocks[heap]);
    }
    return p;
}
