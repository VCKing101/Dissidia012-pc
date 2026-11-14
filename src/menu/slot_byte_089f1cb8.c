typedef struct MenuSlotRef {
    unsigned char field_00;
    signed char field_01;
} MenuSlotRef;

extern signed char func_089EFF4C(void *self, MenuSlotRef *ref);

void func_089F1CB8(void *self, MenuSlotRef *ref) {
    ref->field_01 = func_089EFF4C(self, ref);
}
