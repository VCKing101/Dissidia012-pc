struct MenuHandleGroup {
    int field_00[6];
    int field_18[1];
};

extern "C" {
void func_088DA060(int handle);
void func_0887BA5C(int handle);

void func_089EE9D4(MenuHandleGroup *self) {
    for (int i = 0; i < 6; i++) {
        if (self->field_00[i] != 0) {
            func_088DA060(self->field_00[i]);
            self->field_00[i] = 0;
        }
    }
    for (int i = 0; i < 1; i++) {
        if (self->field_18[i] != 0) {
            func_0887BA5C(self->field_18[i]);
            self->field_18[i] = 0;
        }
    }
}
}
