typedef struct MenuPageState {
    char pad_0000[0x3200];
    int field_3200;
    short field_3204;
    short field_3206;
    char pad_3208[0x4];
    int field_320C;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern void func_089F2D48(MenuPageOwner *self);

void func_089F2BB8(MenuPageOwner *self) {
    self->field_14C->field_3200 = 0;
    self->field_14C->field_3204 = 1;
    self->field_14C->field_3206 = 0;
    self->field_14C->field_320C = -1;
    func_089F2D48(self);
}
