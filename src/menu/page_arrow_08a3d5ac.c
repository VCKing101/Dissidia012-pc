/* Shows the more-pages arrow below six pages (hides it otherwise), then redraws the five rows. */
typedef struct ArrowLabel {
    char pad_00[0x20];
    unsigned short field_20;
} ArrowLabel;

typedef struct ArrowParts {
    char pad_000[0x12C];
    ArrowLabel *field_12C;
} ArrowParts;

typedef struct ArrowState {
    char pad_0000[0x4];
    ArrowParts *field_04;
    char pad_0008[0x29F8 - 0x8];
    int field_29F8;
    int field_29FC;
} ArrowState;

typedef struct ArrowOwner {
    char pad_0000[0x2A14];
    ArrowState *field_2A14;
} ArrowOwner;

extern void func_088D9D24(ArrowLabel *label);
extern void func_08A419E4(void *list);
extern void func_08A419C4(void *list);
extern void func_08A3D49C(ArrowOwner *self, int page, int row);
extern void *D_08A457EC;

void func_08A3D5AC(ArrowOwner *self)
{
    int i;
    if (self->field_2A14->field_29F8 < 6) {
        ArrowLabel *label = self->field_2A14->field_04->field_12C;
        label->field_20 |= 0x4000;
        func_088D9D24(label);
        func_08A419E4(D_08A457EC);
    } else {
        ArrowLabel *label = self->field_2A14->field_04->field_12C;
        label->field_20 &= ~0x4000;
        func_088D9D24(label);
        func_08A419C4(D_08A457EC);
    }
    for (i = 0; i < 5; i++) {
        func_08A3D49C(self, self->field_2A14->field_29FC, i);
    }
}
