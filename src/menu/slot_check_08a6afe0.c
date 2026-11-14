typedef struct SlotItem {
    short field_00;
    short field_02;
    char pad_04[0x2D8];
} SlotItem;

typedef struct SlotPanel {
    char pad_000[0x66C];
    SlotItem *field_66C;
} SlotPanel;

int func_08A6AFE0(SlotPanel *self, int index)
{
    int max = 4;
    SlotItem *item;
    if (index < 0 || max < index) {
        return 0;
    }
    item = &self->field_66C[index];
    if (item->field_02 != 2) {
        return 0;
    }
    if (item->field_00 == 0) {
        return 0;
    }
    return 1;
}
