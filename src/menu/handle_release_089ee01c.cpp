struct MenuHandleSet {
    int field_00[2];
    int field_08[3];
};

extern "C" {
void func_088DA060(int handle);
void func_0887BA5C(int handle);

void func_089EE01C(MenuHandleSet *self) {
    for (int i = 0; i < 2; i++) {
        if (self->field_00[i] != 0) {
            func_088DA060(self->field_00[i]);
            self->field_00[i] = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (self->field_08[i] != 0) {
            func_0887BA5C(self->field_08[i]);
            self->field_08[i] = 0;
        }
    }
}
}
