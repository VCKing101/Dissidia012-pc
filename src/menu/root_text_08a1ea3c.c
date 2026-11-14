typedef struct MenuRootState {
    char pad_00000[0x25B94];
    int field_25B94;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

typedef struct MenuRootView {
    char pad_00[0x68];
    int field_68;
    char pad_6C[0x10];
    int field_7C;
} MenuRootView;

extern unsigned short D_08A23F34[];
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);

void func_08A1EA3C(MenuRootOwner *self, MenuRootView *view) {
    char buffer[64];
    int size;

    func_088DA0A8(view->field_68, D_08A23F34[self->field_154->field_25B94], buffer, &size);
    func_088D9B84(view->field_7C, buffer);
    func_088D9AF4(view->field_7C, &size);
}
