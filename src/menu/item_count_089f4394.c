typedef struct CountItem {
    char pad_00[0xC];
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[0xA];
} CountItem;

int func_089F4394(void *self, CountItem *items)
{
    int count = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (items[i].field_0D != 0 && (items[i].field_0C & 1) != 0) {
            count++;
        }
    }
    return count;
}
