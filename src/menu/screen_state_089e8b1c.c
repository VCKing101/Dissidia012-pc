typedef struct MenuStep {
    int field_00;
    int field_04;
    int field_08;
} MenuStep;

typedef struct MenuStepScreen {
    char pad_000[0x13C];
    int field_13C;
    char pad_140[0xC];
    MenuStep field_14C;
} MenuStepScreen;

extern int func_089ECB70(int handle);

void func_089E8B1C(MenuStepScreen *self) {
    if (func_089ECB70(self->field_13C)) {
        MenuStep *step = &self->field_14C;
        step->field_08 = 0xE;
    }
}
