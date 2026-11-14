typedef struct RefreshItem {
    int field_00;
    unsigned int field_04;
    char pad_08[0xA];
    signed char field_12;
    signed char field_13;
} RefreshItem;

int func_089F1E50(void *self, RefreshItem *item, unsigned char force)
{
    int result = 0;
    if (item->field_13 >= item->field_12 || force) {
        item->field_04 = (item->field_04 | 0x8000) & ~0x400000;
        item->field_13 = item->field_12;
        result = 1;
    }
    return result;
}
