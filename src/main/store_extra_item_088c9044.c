/* Item of extra entry i; 0 when out of range. */
typedef struct Item088C9044 {
    char data[0x24];
} Item088C9044;

typedef struct Store088C9044 {
    char pad_00[0xA180];
    Item088C9044 items[200];
    int used;
    int count;
    unsigned int extra;
} Store088C9044;

unsigned int func_088C9004(Store088C9044 *s, unsigned int i);
Item088C9044 *func_088C9044(Store088C9044 *s, unsigned int i)
{
    unsigned int k;
    if (i >= s->extra) {
        return 0;
    }
    k = func_088C9004(s, i);
    if (k >= 200) {
        return 0;
    }
    return (Item088C9044 *)((char *)s + k * sizeof(Item088C9044) + 0xA180);
}
