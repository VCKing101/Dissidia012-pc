typedef struct SlotItem {
    short field_00;
    short field_02;
    char pad_04[0x2D8];
} SlotItem;

typedef struct SlotPanel {
    char pad_000[0x66C];
    SlotItem *field_66C;
} SlotPanel;

int func_08A6AE24(SlotPanel *self)
{
    int i;
    for (i = 0; i < 5; i++) {
        SlotItem *item = &self->field_66C[i];
        if (item->field_02 == 3 && item->field_00 == 0) {
            return 0;
        }
    }
    return 1;
}
