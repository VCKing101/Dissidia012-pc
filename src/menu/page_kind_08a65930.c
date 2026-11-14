/* Re-derives the page mode from its source kind and refreshes the page. */
typedef struct KindPage {
    char pad_00[0x14];
    int field_14;
    int field_18;
    char pad_1C[0x88 - 0x1C];
    signed char *field_88;
} KindPage;

extern void func_08A65D20(KindPage *self, int value);
extern void func_08A6610C(KindPage *self);

void func_08A65930(KindPage *self)
{
    switch (*self->field_88) {
    case 4:
        self->field_14 = 1;
        break;
    case 5:
        self->field_14 = 2;
        break;
    default:
        self->field_14 = 0;
        break;
    }
    func_08A65D20(self, self->field_18);
    func_08A6610C(self);
}
