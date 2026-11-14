/* Item before the store cursor; 0 when there is none. */
typedef struct Item088CA3C8 {
    char data[0x24];
} Item088CA3C8;

typedef struct Store088CA3C8 {
    char pad_00[0xA180];
    Item088CA3C8 items[200];
    int used;
    int count;
    int extra;
} Store088CA3C8;

int func_088CA1A4(Store088CA3C8 *s);
Item088CA3C8 *func_088CA3C8(Store088CA3C8 *s)
{
    int i = func_088CA1A4(s);
    if (i < 0) {
        return 0;
    }
    if (i - 1 >= 0) {
        return (Item088CA3C8 *)((char *)s + i * (int)sizeof(Item088CA3C8) + 0xA15C);
    }
    return 0;
}
