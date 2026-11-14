typedef struct MenuShopState {
    char pad_000[0x400];
    char field_400[0x480];
    char field_880[0x40C];
    int field_C8C;
} MenuShopState;

typedef struct MenuShopOwner {
    char pad_000[0xF60];
    MenuShopState *field_F60;
} MenuShopOwner;

extern void *D_08A23FF0;
extern int func_08A12F94(MenuShopOwner *self);
extern void func_08A22514(void *global, int mode, MenuShopState *state, char *a, char *b, int c, int d, int e);

void func_08A1230C(MenuShopOwner *self) {
    int result = func_08A12F94(self);
    int mode = result == 0 ? 0xD : 0xE;

    func_08A22514(D_08A23FF0, mode, self->field_F60, self->field_F60->field_400, self->field_F60->field_880, 0, result, 0);
    self->field_F60->field_C8C = 9;
}
