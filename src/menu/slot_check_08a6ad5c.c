typedef struct SlotItem {
    short field_00;
    short field_02;
    char pad_04[0x2D8];
} SlotItem;

typedef struct SlotPanel {
    char pad_000[0x66C];
    SlotItem *field_66C;
} SlotPanel;

int func_08A6AD5C(SlotPanel *self, int index)
{
    int max = 4;
    if (index < 0 || max < index) {
        return 0;
    }
    if (self->field_66C[index].field_02 != 3) {
        return 0;
    }
    return 1;
}
