/* Marks the item released (0x64 = 1), resets it and returns its old id. */
typedef struct KeyedA {
    char pad_00[0x64];
    short released;
    unsigned short id;
} KeyedA;

extern void func_088DCD60(KeyedA *item);

short func_088DCE34(KeyedA *item)
{
    short id = item->id;

    item->released = 1;
    func_088DCD60(item);
    return id;
}
