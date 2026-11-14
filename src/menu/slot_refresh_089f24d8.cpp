struct MenuSlotList {
    char pad_000[0x1F8];
    short field_1F8[10];
    short field_20C[10];
    short field_220;
};

extern "C" {
MenuSlotList *func_088F29C4(void);
int func_089F2448(void *self, int a, int b);
void func_088D4BF4(int sound);

void func_089F24D8(void *self) {
    int changed = 0;
    MenuSlotList *list = func_088F29C4();
    for (int i = 0; i < list->field_220; i++) {
        if (func_089F2448(self, list->field_1F8[i], list->field_20C[i]) != 0) {
            changed = 1;
        }
    }
    if (changed) {
        func_088D4BF4(0x1F);
    }
}
}
