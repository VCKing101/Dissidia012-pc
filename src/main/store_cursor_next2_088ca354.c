/* Second item after the store cursor when it is in use; 0 otherwise. */
typedef struct Item088CA354 {
    char data[0x24];
} Item088CA354;

typedef struct Store088CA354 {
    char pad_00[0xA180];
    Item088CA354 items[200];
    int used;
    int count;
    int extra;
} Store088CA354;

int func_088CA1A4(Store088CA354 *s);
Item088CA354 *func_088CA354(Store088CA354 *s)
{
    int i = func_088CA1A4(s);
    if (i < 0) {
        return 0;
    }
    if (i + 2 < s->used) {
        return (Item088CA354 *)((char *)s + i * (int)sizeof(Item088CA354) + 0xA1C8);
    }
    return 0;
}
