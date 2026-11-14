struct AnyItem {
    char pad_00[0xC];
    short field_0C[3];
};

extern "C" {
int func_08A9BAA0(void *self, AnyItem *item)
{
    int count = 0;
    for (int i = 0; i < 3; i++) {
        short *id = &item->field_0C[i];
        if (*id >= 0) {
            count++;
        }
    }
    if (count == 0) {
        return 0;
    }
    return 1;
}
}
