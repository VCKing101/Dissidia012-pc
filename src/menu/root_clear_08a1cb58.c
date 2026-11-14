typedef struct MenuRootSlot {
    char data[0x1270];
} MenuRootSlot;

typedef struct MenuRootState {
    char pad_0000[0x8E68];
    MenuRootSlot field_8E68[25];
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

extern void func_08935828(void *dst, int value, int size);

void func_08A1CB58(MenuRootOwner *self) {
    int i;
    for (i = 0; i < 25; i++) {
        func_08935828(&self->field_154->field_8E68[i], 0, 0x1270);
    }
}
