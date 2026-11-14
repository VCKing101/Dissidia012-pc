/* Lays out the sections of a block back to back from the base at 0x5C. */
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

void func_088DC7E4(BlockLayout *layout)
{
    int offset = layout->base;

    layout->offset_00 = offset;
    offset += layout->size_3C;
    layout->offset_04 = offset;
    offset += layout->size_40;
    layout->offset_08 = offset;
    offset += layout->size_44;
    layout->offset_0C = offset;
    offset += layout->size_48;
    layout->offset_10 = offset;
    offset += layout->size_4C;
    if (layout->extra_count != 0) {
        layout->extra = offset;
        offset += layout->extra_count * 16;
    } else {
        layout->extra = 0;
    }
    layout->end = offset;
}
