/* Reads a block header (0x24 bytes for version 1, else 0x3C), allocates and lays out the block. */
typedef struct BlockLayout {
    int offset_00;
    int offset_04;
    int offset_08;
    int offset_0C;
    int offset_10;
    int end;
    char header[0x3C - 0x18];
    int size_3C;
    int size_40;
    int size_44;
    int size_48;
    int size_4C;
    int size_50;
    int extra;
    int extra_count;
    int base;
} BlockLayout;

typedef struct BlockHeader {
    int magic;
    int version;
} BlockHeader;

typedef struct HeapOwner {
    int field_00;
    int heap;
} HeapOwner;

extern HeapOwner *D_089D7830;
extern void func_08935524(void *dst, const void *src, int size);
extern char *func_088DC944(BlockLayout *layout, char *data, int *size);
extern char *func_088DCF18(BlockLayout *layout, char *data, int *size, int *size2);
extern char *func_088DD21C(BlockLayout *layout, char *data, int *size, int *size2, int *size3);
extern int func_088DC7A8(BlockLayout *layout);
extern int func_0885C6CC(int heap, int size, int align, int zero, int one, int flags);
extern void func_088DC7E4(BlockLayout *layout);

char *func_088DC84C(BlockLayout *layout, char *data)
{
    int size;

    layout->offset_00 = 0;
    layout->offset_04 = 0;
    layout->offset_08 = 0;
    layout->offset_0C = 0;
    layout->offset_10 = 0;
    layout->extra = 0;
    layout->end = 0;
    if (((BlockHeader *)data)->version == 1) {
        char *next;

        func_08935524(layout->header, data, 0x24);
        data += 0x24;
        next = func_088DC944(layout, data, &layout->size_3C);
        next = func_088DCF18(layout, next, &layout->size_40, &layout->size_44);
        func_088DD21C(layout, next, &layout->size_48, &layout->size_4C, &layout->size_50);
    } else {
        func_08935524(layout->header, data, 0x3C);
        data += 0x3C;
    }
    size = func_088DC7A8(layout);
    {
        int one = 1;

        layout->base = func_0885C6CC(D_089D7830->heap, size, 0, 0, one, 0);
    }
    func_088DC7E4(layout);
    return data;
}
