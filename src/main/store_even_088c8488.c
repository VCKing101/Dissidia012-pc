/* Item for even step n counted from the top; 0 when out of range. */
typedef struct Item088C8488 {
    char data[0x24];
} Item088C8488;

typedef struct Store088C8488 {
    char pad_00[0xA180];
    Item088C8488 items[200];
    int used;
    unsigned int count;
    int extra;
} Store088C8488;

Item088C8488 *func_088C8488(Store088C8488 *s, int n)
{
    unsigned int i;
    if ((unsigned int)(n * 2) >= s->count) {
        return 0;
    }
    i = 199 - n * 2;
    if (i >= 200) {
        return 0;
    }
    return (Item088C8488 *)((char *)s->items + i * sizeof(Item088C8488));
}
