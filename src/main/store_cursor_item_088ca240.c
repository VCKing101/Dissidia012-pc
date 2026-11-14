/* Item at the store cursor; 0 when there is none. */
typedef struct Item088CA240 {
    char data[0x24];
} Item088CA240;

typedef struct Store088CA240 {
    char pad_00[0xA180];
    Item088CA240 items[200];
    int used;
    int count;
    int extra;
} Store088CA240;

int func_088CA1A4(Store088CA240 *s);
Item088CA240 *func_088CA240(Store088CA240 *s)
{
    int i = func_088CA1A4(s);
    if (i < 0) {
        return 0;
    }
    return (Item088CA240 *)((char *)s + i * (int)sizeof(Item088CA240) + 0xA180);
}
