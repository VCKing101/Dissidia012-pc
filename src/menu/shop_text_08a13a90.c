typedef struct MenuShopState {
    char pad_000[0x4B0];
    int field_4B0;
    char pad_4B4[0x4F8 - 0x4B4];
    int field_4F8;
    char pad_4FC[0x514 - 0x4FC];
    int field_514;
    char pad_518[0x8];
    int field_520;
} MenuShopState;

typedef struct MenuShopOwner {
    char pad_000[0xF60];
    MenuShopState *field_F60;
} MenuShopOwner;

extern unsigned short D_08A15E98[];
extern unsigned short D_08A15E9C[];
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);

void func_08A13A90(MenuShopOwner *self, int which, int index) {
    char buffer[64];
    int size;

    if (which == 0) {
        func_088DA0A8(self->field_F60->field_514, D_08A15E98[index], buffer, &size);
        func_088D9B84(self->field_F60->field_520, buffer);
        func_088D9AF4(self->field_F60->field_520, &size);
    } else {
        func_088DA0A8(self->field_F60->field_4B0, D_08A15E9C[index], buffer, &size);
        func_088D9B84(self->field_F60->field_4F8, buffer);
        func_088D9AF4(self->field_F60->field_4F8, &size);
    }
}
