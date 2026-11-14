typedef struct KeyedB {
    char pad_00[4];
    short count;
    short field_06;
    unsigned short id;
} KeyedB;

extern void func_088DE1A8(KeyedB *item);

short func_088DE224(KeyedB *item)
{
    short id = item->id;

    item->count = 1;
    func_088DE1A8(item);
    return id;
}
