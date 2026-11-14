/* Copies `size` bytes into a new block from `heap` and appends it to the list at 8. */
typedef struct BlockList {
    char pad_00[8];
    void *blocks[(0x5C - 8) / 4];
    int count;
} BlockList;

extern void *func_0885C6CC(int heap, int size, int align, int zero, int one, int flags);
extern void func_08935524(void *dst, const void *src, int size);

void *func_088E2AFC(BlockList *list, int heap, const void *src, int size)
{
    int one = 1;
    void *block = func_0885C6CC(heap, size, 0, 0, one, 0);

    list->blocks[list->count] = block;
    func_08935524(block, src, size);
    return list->blocks[list->count++];
}
