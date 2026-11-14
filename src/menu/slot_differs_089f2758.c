typedef struct MenuSlotRange {
    char pad_00[0x8];
    short field_08;
    short field_0A;
} MenuSlotRange;

int func_089F2758(void *self, MenuSlotRange *range) {
    if (range->field_08 != range->field_0A) {
        return 1;
    }
    return 0;
}
