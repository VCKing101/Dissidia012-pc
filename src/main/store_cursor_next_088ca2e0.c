/* Item after the store cursor when it is in use; 0 otherwise. */
typedef struct Item088CA2E0 {
    char data[0x24];
} Item088CA2E0;

typedef struct Store088CA2E0 {
    char pad_00[0xA180];
    Item088CA2E0 items[200];
    int used;
    int count;
    int extra;
} Store088CA2E0;

int func_088CA1A4(Store088CA2E0 *s);
Item088CA2E0 *func_088CA2E0(Store088CA2E0 *s)
{
    int i = func_088CA1A4(s);
    if (i < 0) {
        return 0;
    }
    if (i + 1 < s->used) {
        return (Item088CA2E0 *)((char *)s + i * (int)sizeof(Item088CA2E0) + 0xA1A4);
    }
    return 0;
}
