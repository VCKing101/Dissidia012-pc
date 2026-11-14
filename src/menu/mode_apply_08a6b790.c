/* Applies the panel's mode (1, 2 or 3) by refreshing the parts it uses. */
typedef struct ApplyPanel2 {
    char pad_00[0x28];
    int field_28;
} ApplyPanel2;

extern void func_08A6B91C(ApplyPanel2 *self);
extern void func_08A6B8C8(ApplyPanel2 *self);
extern void func_08A6BA90(ApplyPanel2 *self);

void func_08A6B790(ApplyPanel2 *self)
{
    switch (self->field_28) {
    case 1:
        func_08A6B91C(self);
        func_08A6B8C8(self);
        break;
    case 2:
        func_08A6B8C8(self);
        break;
    case 3:
        func_08A6B8C8(self);
        func_08A6BA90(self);
        break;
    }
}
