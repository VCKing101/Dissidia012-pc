typedef struct MenuShopState {
    char pad_000[0x510];
    int field_510;
    char pad_514[0xC];
    int field_520;
    int field_524[3];
    char pad_530[0xEDC - 0x530];
    int field_EDC;
    int field_EE0;
} MenuShopState;

typedef struct MenuShopOwner {
    char pad_000[0xF60];
    MenuShopState *field_F60;
} MenuShopOwner;

extern "C" {
void func_088D9CFC(int handle);

void func_08A13624(MenuShopOwner *self) {
    self->field_F60->field_EDC = self->field_F60->field_EE0;
    func_088D9CFC(self->field_F60->field_510);
    func_088D9CFC(self->field_F60->field_520);
    for (int i = 0; i < 3; i++) {
        func_088D9CFC(self->field_F60->field_524[i]);
    }
}
}
