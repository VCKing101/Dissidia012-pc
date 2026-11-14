typedef struct MenuSlotKey {
    short field_00;
    short field_02;
    short field_04;
} MenuSlotKey;

typedef struct MenuSlotView {
    char pad_00[0xC];
    short field_0C;
} MenuSlotView;

extern void func_089F166C(void *self, MenuSlotKey *key, MenuSlotView *view, int mode);

void func_089F1980(void *self, MenuSlotKey *key, MenuSlotView *view) {
    if (view->field_0C != -1) {
        if (key->field_04 != -1) {
            func_089F166C(self, key, view, 1);
        }
    } else {
        func_089F166C(self, key, view, 1);
    }
}
