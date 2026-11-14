/* Item for odd step n counted from the top; 0 when out of range. */
typedef struct Item088C84E8 {
    char data[0x24];
} Item088C84E8;

typedef struct Store088C84E8 {
    char pad_00[0xA180];
    Item088C84E8 items[200];
    int used;
    unsigned int count;
    int extra;
} Store088C84E8;

Item088C84E8 *func_088C84E8(Store088C84E8 *s, int n)
{
    unsigned int k = n * 2 + 1;
    unsigned int i;
    if (k >= s->count) {
        return 0;
    }
    i = 199 - k;
    if (i >= 200) {
        return 0;
    }
    return (Item088C84E8 *)((char *)s->items + i * sizeof(Item088C84E8));
}
