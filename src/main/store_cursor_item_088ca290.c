/* Item at the store cursor; 0 when there is none. */
typedef struct Item088CA290 {
    char data[0x24];
} Item088CA290;

typedef struct Store088CA290 {
    char pad_00[0xA180];
    Item088CA290 items[200];
    int used;
    int count;
    int extra;
} Store088CA290;

int func_088CA1A4(Store088CA290 *s);
Item088CA290 *func_088CA290(Store088CA290 *s)
{
    int i = func_088CA1A4(s);
    if (i < 0) {
        return 0;
    }
    return (Item088CA290 *)((char *)s->items + i * (int)sizeof(Item088CA290));
}
