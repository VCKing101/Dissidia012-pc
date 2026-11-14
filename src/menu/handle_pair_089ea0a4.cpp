struct MenuHandlePair {
    int field_00;
    int field_04;
    int field_08[2];
};

extern "C" {
void func_088DA060(int handle);

void func_089EA0A4(MenuHandlePair *self) {
    for (int i = 0; i < 2; i++) {
        if (self->field_08[i] != 0) {
            func_088DA060(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
}
}
